// Alex Dombos
// 2015-12-10
// Purpose: Quickly apply normalized scaling parameters to
//          spectra to look at results
//
// A. Tsantiri - 2022-12-2
// Now chi2 fits TAS, SOS, Mul, diagonal Ex-Eg projection of 2D matrix
//
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

double integral_above_zero(TH1D* hist){
  double output = 0;
  for (int i=1; i<=hist->GetNbinsX(); i++){ // [1, NbinsX] inclusive
    double bin_content = hist->GetBinContent(i);
    if (bin_content > 0){
      output += bin_content;
    }
  }
  return output;
}

void scale_above_zero(TH1D* hist, double integral){
  double hist_integral_above_zero = integral_above_zero(hist);
  
  for (int i=1; i<=hist->GetNbinsX(); i++){ // [1, NbinsX] inclusive
    double bin_content = hist->GetBinContent(i);
    if (bin_content > 0){
      hist->SetBinContent(i, bin_content / hist_integral_above_zero * integral);
    }
  }
}

TCanvas* create_canvas(TH1D* hist_exp, TH1D* hist_sim, string fit_number){

  // Ensure the histograms have the same binning, xmin, xmax
  assert(hist_exp->GetNbinsX() == hist_sim->GetNbinsX());
  assert(hist_exp->GetXaxis()->GetXmin() == hist_sim->GetXaxis()->GetXmin());
  assert(hist_exp->GetXaxis()->GetXmax() == hist_sim->GetXaxis()->GetXmax());

  TCanvas* canvas = new TCanvas(Form("c_%s",hist_exp->GetName()),
				Form("c_%s",hist_exp->GetName()));
  
  canvas->Divide(1,2);
  canvas->cd(1);
  if (std::string(hist_exp->GetName()) == "data_Seg") {
    cout<<"test"<<endl;
    gPad->SetLogy();
  }

  std::cout << "Normalizing: "
	    << hist_exp->GetName() << " and "
	    << hist_sim->GetName() << std::endl;

  //////////
  // Ignore negative counts (which might occur after subtraction of random spectra) in the integral
  //////////
  
  double integral = TMath::Min(integral_above_zero(hist_exp),
			       integral_above_zero(hist_sim));

  std::cout << "Unnormalized Integrals" << std::endl;
  std::cout << "hist_exp:   " << integral_above_zero(hist_exp) << std::endl;
  std::cout << "hist_sim:   " << integral_above_zero(hist_sim) << std::endl;
  std::cout << "integral:   " << integral << std::endl;

  //hist_exp->Scale(integral / integral_above_zero(hist_exp));
  scale_above_zero(hist_exp, integral);
  hist_exp->SetLineColor(1);
  hist_exp->Draw("same hist");
  // hist_sim->Scale(integral / integral_above_zero(hist_sim));
  scale_above_zero(hist_sim, integral);
  hist_sim->SetLineColor(2);
  hist_sim->Draw("same hist");

  std::cout << "Normalized Integrals" << std::endl;
  std::cout << "hist_exp:   " << integral_above_zero(hist_exp) << std::endl;
  std::cout << "hist_sim:   " << integral_above_zero(hist_sim) << std::endl;
  
  canvas->cd(2);

  TH1D* error = new TH1D(Form("error_%s",hist_exp->GetName()), "",
			 hist_exp->GetNbinsX(),
			 hist_exp->GetXaxis()->GetXmin(),
			 hist_exp->GetXaxis()->GetXmax());
  for (int i=1; i<=hist_exp->GetNbinsX(); i++){ // [1, NbinsX] inclusive
    double exp = hist_exp->GetBinContent(i);
    double sim = hist_sim->GetBinContent(i);
    double percent_error = (sim - exp) / exp * 100; // percent_error = relative_error * 100

    if (exp > 0){
      error->SetBinContent(i, percent_error);
    }
  }
  
  error->SetLineColor(4);
  error->Draw("same");

  canvas->SaveAs(Form("figures/c_%s%s.png",hist_exp->GetName(),fit_number.c_str()));
  return canvas;

}

int draw_results(std::string fit_number, std::string input_sim_name){

  // ========== Parse input file to get scaling factors

  std::ifstream input_file(Form("output%s.txt", fit_number.c_str()));
  if (!input_file){
    std::cerr << "Could not open file" << std::endl;
    return -1;
  }
  std::cout << "Input Text File: " << Form("output%s.txt", fit_number.c_str()) << std::endl;

  // Scaling factors
  std::string nPar;
  int parameter_number;
  double scaling_factor;
  std::string error;
  double error_value;
  
  std::vector<double> normalized_scaling_factors;
  std::vector<double> normalized_errors;
  std::string line;
  while (std::getline(input_file, line)){
    std::istringstream iss(line);
    //std::cout << "line: " << line << std::endl;
    if (iss >> nPar >> parameter_number >> scaling_factor >> error >> error_value){
      if (nPar == "nPar"){
	normalized_scaling_factors.push_back(scaling_factor);
	normalized_errors.push_back(error_value);
      }
    }
  }

  // ========== Apply normalized scaling factors 
  string input_exp_name = "exp_input_run_1205.root";
  TFile* fInExp = new TFile(input_exp_name.c_str(),"READ"); 
  TH1D* exp_TAS = (TH1D*) fInExp->Get("data_TAS");
  TH1D* exp_Seg = (TH1D*) fInExp->Get("data_Seg");
  TH1D* exp_Mul = (TH1D*) fInExp->Get("data_Mul");
  // TH1D* exp_Diag= (TH1D*) fInExp->Get("data_Diag");

  // string input_sim_name = "sim_hsts_Jp_T1.0.root";
  TFile*  fInSim = new TFile(input_sim_name.c_str(),"READ");

  TH1D* sim_TAS = new TH1D("sim_TAS","",exp_TAS->GetNbinsX(),exp_TAS->GetXaxis()->GetXmin(),exp_TAS->GetXaxis()->GetXmax());
  TH1D* sim_Seg = new TH1D("sim_Seg","",exp_Seg->GetNbinsX(),exp_Seg->GetXaxis()->GetXmin(),exp_Seg->GetXaxis()->GetXmax());
  TH1D* sim_Mul = new TH1D("sim_Mul","",exp_Mul->GetNbinsX(),exp_Mul->GetXaxis()->GetXmin(),exp_Mul->GetXaxis()->GetXmax());
  // TH1D* sim_Diag= new TH1D("sim_Diag","",exp_Diag->GetNbinsX(),exp_Diag->GetXaxis()->GetXmin(),exp_Diag->GetXaxis()->GetXmax());

  for (unsigned int i=0; i<normalized_scaling_factors.size(); i++){
    
    if (i==0){
      std::cout << "First normalized scaling factor being used: " << std::setprecision(25) << normalized_scaling_factors[i] << std::endl;
      //std::cout << "First normalized error being used:          " << std::setprecision(25) << normalized_errors[i] << std::endl;
    }
    if (i==(normalized_scaling_factors.size()-1)){
      std::cout << "Last normalized scaling factor being used:  " << std::setprecision(25) << normalized_scaling_factors[i] << std::endl;
      //std::cout << "Last normalized error being used:           " << std::setprecision(25) << normalized_errors[i] << std::endl;
    }
    std::cout<<"i: "<<i<<std::endl; 
    sim_TAS->Add((TH1D*) fInSim->Get(Form("c_%d_TAS",i)), normalized_scaling_factors[i]);

    sim_Seg->Add((TH1D*) fInSim->Get(Form("c_%d_Seg",i)), normalized_scaling_factors[i]);

    sim_Mul->Add((TH1D*) fInSim->Get(Form("c_%d_Mul",i)), normalized_scaling_factors[i]);

    // sim_Diag->Add((TH1D*) fInSim->Get(Form("c_%d_Diag",i)), normalized_scaling_factors[i]);

  }

  // ========== Draw Results-->Quick Review
    
  TCanvas* c1 = create_canvas(exp_TAS, sim_TAS, fit_number);
  TCanvas* c2 = create_canvas(exp_Seg, sim_Seg, fit_number);
  TCanvas* c3 = create_canvas(exp_Mul, sim_Mul, fit_number);
  // TCanvas* c4 = create_canvas(exp_Diag, sim_Diag);
	TFile* fOut = new TFile("comparison.root","UPDATE");
	// exp_Seg->Write();
	// exp_TAS->Write();
	// exp_Mul->Write();
  sim_Seg->Write("sim_Seg_Tdef");
  sim_TAS->Write("sim_TAS_Tdef");
  sim_Mul->Write("sim_Mul_Tdef");
  // sim_Seg->Write(Form("sim_Seg%s",fit_number.c_str()));
  // sim_TAS->Write(Form("sim_TAS%s",fit_number.c_str()));
  // sim_Mul->Write(Form("sim_Mul%s",fit_number.c_str()));
  fOut->Close();
 
  return 0;
  
}
