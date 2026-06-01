// A. Tsantiri - 2022-12-2
// Frankenstein the shit out of Alex's code for ND_Ni analysis.
// Now chi2 will try to fit TAS, SOS, Mul, diagonal Ex-Eg projection of 2D matrix
//	    spectra_Diag is reserved for "Diag"
//
// Alex Dombos
// 2015-11-06
// Purpose: To perform multidimensional chi2 minimization on 
//          spectra from beta-decay experiments with SuN.
//
//          spectra_TAS is reserved for "TAS"
//          spectra_Seg is reserved for "Seg"
//          spectra_Mul is reserved for "Mul"
//          For spectra that are not being fit, leave
//          that function argument as ""
//
//          Use either ROOT/ACLiC or compile into an executable.
//          Depending on which is chosen, the corresponding 
//          code at the beginning has to be commented/uncommented
//          (that is, the sections "int fit_spectra" and "int main")
// 
//          If using ROOT/ACLiC,
//          root [0] .L fit_spectra.C++g
//          root [1] fit_spectra(enter arguments)
//
//          If compiling,
//          g++ fit_spectra.cpp $(root-config --cflags) $(root-config --glibs) -std=c++11 -O3 -o fit_spectra
//          ./fit_spectra command_line_arguments         
//
//          Many fits can be performed simultaneously in
//          parallel by sending them to Seaside.
//
//
// Below is information from
// "NumericalMinimization.C: Example on how to use the new Minimizer class in ROOT"
// https://root.cern.ch/root/html/tutorials/fit/NumericalMinimization.C.html
//
// Possible combinations of minimizer and algorithm names are
// (some minimizers do not have algorithms)
// Minimizer             Algorithm
// Minuit / Minuit2      Migrad, Simplex, Combined, Scan
// Minuit2               Fumili2
// Fumili
// GSLMultiMin           ConjugateFR, ConjugatePR, BFGS
//                       BFGS2, SteepestDescent
// GSLMultiFit
// GSLSimAn
// Genetic

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>

#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

// Number of experimental spectra to fit and number of components
int global_num_histograms_to_fit;
int global_num_components;

// Global variables for experiment and simulation
std::vector<TH1D*>  exp_histograms;
std::vector<std::vector<TH1D*> > sim_histograms;

// Global chi2 value and number of bins
double global_chi2;
double global_nBins;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string current_date_time() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  
  return buf;
}

void error_analysis(const double* minimum_parameters,
		   std::vector<double>& normalized_minimum_parameters,
		   const double* minimum_errors,
		   std::vector<double>& normalized_minimum_errors){

  // For normalizing the output parameters and determining the associated errors
  double minimum_parameters_sum = 0.0;
  double minimum_parameters_sum_error = 0.0;
  for (int i=0; i<global_num_components; i++){
    minimum_parameters_sum += minimum_parameters[i];
    minimum_parameters_sum_error += (minimum_errors[i] * minimum_errors[i]);
  }
  minimum_parameters_sum_error = TMath::Sqrt(minimum_parameters_sum_error);
  
  for (int i=0; i<global_num_components; i++){
    normalized_minimum_parameters.push_back(minimum_parameters[i]/minimum_parameters_sum);
    normalized_minimum_errors.push_back(normalized_minimum_parameters[i]
					* TMath::Sqrt(TMath::Power((minimum_errors[i]) / (minimum_parameters[i]),2) +
						      TMath::Power((minimum_parameters_sum_error) / (minimum_parameters_sum),2)));
  }
}

std::string minimizer_status(int minimizer_status_value){
  
  // Minimizer status
  // https://root.cern.ch/root/html532/ROOT__Minuit2__Minuit2Minimizer.html
  
  if (minimizer_status_value == 0){
    return "Minimization converged";
  }
  if (minimizer_status_value == 1){
    return "Covariance was made positive definite";
  }
  if (minimizer_status_value == 2){
    return "Hesse is invalid";
  }
  if (minimizer_status_value == 3){
    return "Estimated distance to minimum is above max";
  }
  if (minimizer_status_value == 4){
    return "Reached call limit";
  }
  if (minimizer_status_value == 5){
    return "Any other failure";
  }
  return "There has been an error";
}

// Given a data and hypothesis histogram, loop over the bins and calculate Pearson's chi-squared statistic
// (Really the modified least-squares method, since we divide by obs instead of exp in the chi2 calculation - "Statistical Data Analysis" by Cowan, pg 101)
// Variable discriptions
// data = The experimental histogram you are trying to fit
// components = A vector of simulation histograms that are used as components to fit the experimental histogram
// parameters = An array of parameters / scaling factors that are applied to the components. The parameters are constantly being varied during the minimization
// bin_count = The number of bins included in the fit
double calculate_chi2(TH1D* data, std::vector<TH1D*> components, const double* parameters, double& bin_count){
  
  double chi2 = 0;
  bin_count = 0;

  // [1, NbinsX] inclusive to avoid underflow/overflow bins
  for (int i=1; i<=data->GetNbinsX(); i++){
    double obs = data->GetBinContent(i); // obs = observed
    double exp = 0;                      // exp = expected (ie simulation, not experiment!)
    if (obs > 0){                        // "Statistical Data Analysis" by Cowan, pgs 61-63 and 101-102 about whether this should be >=5
      for (int j=0; j<global_num_components; j++){
	exp += parameters[j] * (components[j]->GetBinContent(i));
      }
      chi2 += (obs - exp) * (obs - exp) / obs;
      ++ bin_count;
    }
  }
  return chi2;
}
// The function that will be minimized
double fitting_function(const double* parameters){

  std::vector<double> bins(global_num_histograms_to_fit);  // Stores the number of bins included in the fit from each experimental spectra. Important for reduced chi2.
  std::vector<double> chi2_values(global_num_histograms_to_fit); // Stores the chi2 values from each experimental spectra

  // Find the local chi2 for each experimental spectrum being fit
  for (int i=0; i<global_num_histograms_to_fit; i++){
    chi2_values[i] = calculate_chi2(exp_histograms[i], sim_histograms[i], parameters, bins[i]);
  }

  // Sum the contibutions from each function (calculate the global chi2)
  global_chi2  = 0;
  global_nBins = 0;
  for (int i=0; i<global_num_histograms_to_fit; i++){
    global_chi2 += chi2_values[i];
    global_nBins += bins[i];
    //std::cout << Form("chi2_values[%d]:  ",i) << chi2_values[i] << std::endl;
    //std::cout << Form("bins[%d]:         ",i) << bins[i]  << std::endl;
  }
  
  return global_chi2;

  //std::cout << "global_chi2:  " << global_chi2 << std::endl;
  //std::cout << "global_nBins: " << global_nBins << std::endl;
}

int fit_spectra(std::string fit_number,
		int input_num_histograms_to_fit,
		int input_num_components,
		std::string input_sim_name, // input simulated histogram name
		std::string input_exp_name, // input experimental histogram name
		std::string spectra_TAS,
		std::string spectra_Seg,
		std::string spectra_Mul,
		std::string minimizer_name = "Minuit",
		std::string algorithm_name = "Migrad", // Minuit's best minimization algorithm
		int num_max_iterations = 1E5,
		int num_max_function_calls = 1E6,
		double tolerance = 0.001,
		double error_definition = 1.0,
		int print_level = 0){
  
  // Set global variables
  global_num_histograms_to_fit = input_num_histograms_to_fit;
  global_num_components = input_num_components;

  // Get timing information
  std::string start_date = current_date_time();
  clock_t tStart = clock();
  std::cout << "Start Date: " << current_date_time() << std::endl;

  // Make sure function arguments were input correctly
  if (spectra_TAS == "Seg" ||
      spectra_TAS == "Mul" ||
      spectra_Seg == "TAS" ||
      spectra_Seg == "Mul" ||
      spectra_Mul == "TAS" ||
      spectra_Mul == "Seg" 
    ){
    std::cerr << "There has been an input error with spectra names" << std::endl;
    return -1;
  }

  // Safety counter to make sure function arguments were input correctly
  int input_counter = 0;

  // Input ROOT files
  // string input_exp_name = "/mnt/projects/e22505/aMinor/chi2_analysis/subtracted_histograms_3.29.root";
  TFile* fInData = TFile::Open(input_exp_name.c_str(),"READ");
  if (!fInData){
    std::cerr << "Could not open file with experimental spectra" << std::endl;
    return -1;
  }

  // string input_sim_name = "sim_hsts_74Se.root";
  TFile* fInSim = TFile::Open(input_sim_name.c_str(),"READ");
  if (!fInSim){
    std::cerr << "Could not open file with simulation spectra" << std::endl;
    return -1;
  }

  std::cout<<"Input exp file: "<<input_exp_name<<std::endl;
  std::cout<<"Input sim file: "<<input_sim_name<<std::endl;
  // The experimental histograms to be fit
  TH1D* exp_TAS = nullptr;
  TH1D* exp_Seg = nullptr;
  TH1D* exp_Mul = nullptr;

  fInData->cd();
  if (spectra_TAS == "TAS"){
    exp_TAS = (TH1D*) fInData->Get("data_TAS");
    if (!exp_TAS){
      std::cerr << "Experimental TAS spectrum does not exist" << std::endl;
      return -1;
    }
    ++ input_counter;
  }
  if (spectra_Seg == "Seg"){
    exp_Seg = (TH1D*) fInData->Get("data_Seg");
    if (!exp_Seg){
      std::cerr << "Experimental Seg spectrum does not exist" << std::endl;
      return -1;
    }
    ++ input_counter;
  }
  if (spectra_Mul == "Mul"){
    exp_Mul = (TH1D*) fInData->Get("data_Mul");
    if (!exp_Mul){
      std::cerr << "Experimental Mul spectrum does not exist" << std::endl;
      return -1;
    }
    ++ input_counter;
  }

  // Make sure function arguments were input correctly 
  if (global_num_histograms_to_fit != input_counter){
    std::cout<<global_num_histograms_to_fit<<input_counter<<std::endl;
    std::cerr << "There has been an input error with the number of components" << std::endl;
    return -1;
  }

  // The simulation component histograms
  std::vector<TH1D*> row_sim_TAS;
  std::vector<TH1D*> row_sim_Seg;
  std::vector<TH1D*> row_sim_Mul;
  for (int i=0; i<global_num_components; i++){
    if (spectra_TAS == "TAS"){
      row_sim_TAS.push_back((TH1D*) fInSim->Get(Form("c_%d_TAS",i)));
      if (!row_sim_TAS.back()){
	std::cerr << "Simulated TAS component does not exist" << std::endl;
	std::cout << fInSim->Get(Form("c_%d_TAS",i)) << std::endl;
	return -1;
      }
    }
    if (spectra_Seg == "Seg"){
      row_sim_Seg.push_back((TH1D*) fInSim->Get(Form("c_%d_Seg",i)));
      if (!row_sim_Seg.back()){
	std::cerr << "Simulated Seg component does not exist" << std::endl;
	return -1;
      }
    }
    if (spectra_Mul == "Mul"){
      row_sim_Mul.push_back((TH1D*) fInSim->Get(Form("c_%d_Mul",i)));
      if (!row_sim_Mul.back()){
	std::cerr << "Simulated Mul component does not exist" << std::endl;
	return -1;
      }
    }
  }

  // Vector to hold the experimental spectra, and matrix to hold the simulation spectra
  if (spectra_TAS == "TAS"){
    exp_histograms.push_back(exp_TAS);
    sim_histograms.push_back(row_sim_TAS);
  }
  if (spectra_Seg == "Seg"){
    exp_histograms.push_back(exp_Seg);
    sim_histograms.push_back(row_sim_Seg);
  }
  if (spectra_Mul == "Mul"){
    exp_histograms.push_back(exp_Mul);
    sim_histograms.push_back(row_sim_Mul);
  }

  // Ensure experimental histograms and corresponding simulation components have the same binning, xmin, and xmax
  for (unsigned int i=0; i<exp_histograms.size(); i++){
    for (unsigned int j=0; j<sim_histograms[i].size(); j++){
      //std::cout << "i: " << i << " j: " << Form("%03d",j) << std::endl;
      assert(exp_histograms[i]->GetNbinsX() == sim_histograms[i][j]->GetNbinsX());
      assert(exp_histograms[i]->GetXaxis()->GetXmin() == sim_histograms[i][j]->GetXaxis()->GetXmin());
      assert(exp_histograms[i]->GetXaxis()->GetXmax() == sim_histograms[i][j]->GetXaxis()->GetXmax());
    }
  }
  
  // Initialize the minimizer and algorithm
  ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer(minimizer_name, algorithm_name);
  ROOT::Math::Functor minimization_function(&fitting_function, global_num_components);

  // Set minimizer properties
  minimizer->SetPrintLevel(print_level); // Set the print level. A print level of 0 prevents all output during the minimization process. The higher the number, the more often you will see the minimzer print what is happening during the minimization process.
  minimizer->SetMaxIterations(num_max_iterations); // Set maximum iterations (one iteration can have many function calls) (for GSL)
  minimizer->SetMaxFunctionCalls(num_max_function_calls); // Set maximum number of function calls (for Minuit/Minuit2)
  minimizer->SetTolerance(tolerance); // Set the tolerance 
  minimizer->SetErrorDef(error_definition); // Set scale for calculating the errors. For example, 1 would set the  error level to 1 sigma (1 for chi2, 0.5 for loglikelihood)
  minimizer->SetFunction(minimization_function); // Set the function to minimize

  // Parameters for the minimizer
  std::vector<std::string> parameter_names; // Name of parameters
  std::vector<double> start_values;         // Start values
  std::vector<double> step_sizes;           // Step sizes
  std::vector<double> lower_bounds;         // Lower bounds on parameter
  std::vector<double> upper_bounds;         // Upper bounds on parameter

  // Set parameter values
  for (int i=0; i<global_num_components; i++){
    parameter_names.push_back(Form("parameter_%03d",i));
    start_values.push_back(0.1/global_num_components); // Guess at true value
    step_sizes.push_back(0.001); // This is just a starting value as Minuit will adjust this if necessary
    lower_bounds.push_back(0.00000);
    upper_bounds.push_back(0.20);
  }


  // Set parameters
  for (int i=0; i<global_num_components; i++){
    minimizer->SetLimitedVariable(i, parameter_names[i].c_str(), start_values[i], step_sizes[i], lower_bounds[i], upper_bounds[i]);
  }

  // Minimize
  minimizer->Minimize(); // Method to perform the minimization
 
  // Date the fit ended
  std::string end_date = current_date_time();
  
  // Output file to save the results
  std::string output_file_name = Form("output%s.txt", fit_number.c_str());
  std::ofstream output_file(output_file_name.c_str(), std::ios::app);

  std::cout<<"Output file: "<<output_file_name.c_str()<<std::endl;
  std::cout<< "Status: " << minimizer->Status() << " (" << minimizer_status(minimizer->Status()) << ")" <<std::endl;

  // Save header information
  output_file << "Start Date:                      " << start_date << std::endl;
  output_file << "End Date:                        " << end_date << std::endl;
  output_file << "Elapsed Time:                    " << (double)(clock() - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;
  output_file << "Number of fit spectra:           " << global_num_histograms_to_fit << " (" << spectra_TAS << " " << spectra_Seg << " " << spectra_Mul << ")" << std::endl;
  output_file << "Number of components:            " << global_num_components << std::endl;
  output_file << "Free and constrained variables:  " << minimizer->NDim() << std::endl;
  output_file << "Free variables:                  " << minimizer->NFree() << std::endl;

  output_file << "Minimizer:                       " << minimizer_name << std::endl;
  output_file << "Algorithm:                       " << algorithm_name << std::endl; 
  output_file << "Max Iterations:                  " << num_max_iterations << std::endl; // minimizer->MaxIterations()
  output_file << "Max Function Calls:              " << minimizer->MaxFunctionCalls() << std::endl;
  output_file << "Actual Number of Function Calls: " << minimizer->NCalls() << std::endl;
  output_file << "Tolerance:                       " << minimizer->Tolerance() << std::endl;
  output_file << "Precision:                       " << minimizer->Precision() << std::endl;
  output_file << "Error Definition:                " << minimizer->ErrorDef() << std::endl;
  output_file << "Estimated Distance To Minimum:   " << minimizer->Edm() << std::endl; // Supposedly the fit has converged if Edm < 0.001 * tolerance
  output_file << "Print Level:                     " << minimizer->PrintLevel() << std::endl;

  output_file << "Status:                          " << minimizer->Status() << " (" << minimizer_status(minimizer->Status()) << ")" <<std::endl;
  output_file << "Minimum Value:                   " << minimizer->MinValue() << std::endl;

  output_file << "global_chi2:                     " << global_chi2 << std::endl;
  output_file << "global_nBins:                    " << global_nBins << std::endl;

  output_file << "Reduced global chi2:             " << global_chi2 / (global_nBins - minimizer->NFree()) << std::endl;

  // Get results and save them
  const double* minimum_parameters = minimizer->X();      // Pointer to the parameters at the minimum
  const double* minimum_errors = minimizer->Errors(); // Pointer to the errors at the minimum
  
  // Normalize parameters and obtain errors
  std::vector<double> normalized_minimum_parameters;
  std::vector<double> normalized_minimum_errors;
  error_analysis(minimum_parameters, normalized_minimum_parameters, minimum_errors, normalized_minimum_errors);

  // Unnormalized parameters
  for (int i=0; i<global_num_components; i++){
    output_file << "Par " << std::right << std::setfill('0') << std::setw(3) << i << "    " << std::left << std::setfill(' ') << std::setw(15) << minimum_parameters[i] 
		<< "error: " << std::left << std::setw(15) << minimum_errors[i] << "percent error: " << std::left << std::setw(15) << minimum_errors[i]/minimum_parameters[i] << std::endl;
  }
  // Normalized parameters
  for (int i=0; i<global_num_components; i++){
    output_file << "nPar " << std::right << std::setfill('0') << std::setw(3) << i << "    " << std::left << std::setfill(' ') << std::setw(35) << std::setprecision(25) << normalized_minimum_parameters[i] 
		<< "error: " << std::left << std::setw(35) << std::setprecision(25) << normalized_minimum_errors[i] << "percent error: " << std::left << std::setw(35) << std::setprecision(25) << normalized_minimum_errors[i]/normalized_minimum_parameters[i] << std::endl;
  }

  // Finished
  minimizer->Clear();
  output_file.close();
  
  fInData->Close();
  delete fInData;

  fInSim->Close();
  delete fInSim;

  return 0;

}
