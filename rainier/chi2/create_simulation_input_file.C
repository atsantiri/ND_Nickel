
// Alex Dombos
// 2017-04-25
// Purpose: To create the simulation input files
//          needed for the chi2 minimization code.
//          TH1::SetDirectory(0) is used because
//          the histogram is stored in a vector, but the
//          the file is deleted. And the file is deleted
//          to prevent std::bad_alloc
//
//          *****
//          Isotope dependent variables
//          1) the directory where the root files are stored
//          2) the gates (must be identical to experiment)
//          3) the binning of the spectra (to match the binning of the experimental spectra to be fit)
//          4) the number of components (loop over i)
//             a) for simulations of the individual cascades
//             of the known, low-lying level scheme, the number of components should only go up to
//             the critical energy in dice box
//             b) for simulations using components from dice box, check the loop over i,
//             the loop over j, and the variables component_number and number_of_spins
//          *****
//
//          To run:
//          root[0] .L create_simulation_input_files.C++g
//          root[1] create_simulation_input_files()
//
//
// A. Tsantiri - 2022-12-2
// Frankenstein the shit out of Alex's code for ND_Ni analysis based on D. Muecher ShapeMatrix.C ln 480 - 495.
// Now chi2 will try to fit TAS, SOS, Mul, diagonal Ex-Eg projection of 2D matrix
//
// Mod for e22505 - not using diag and 2D matrix yet

#include <iostream>
#include <cassert>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

TH1D *make_sos_allsegs(TTree *tree, int min, int max, string name, int bins, int low, int high)
{

	TH1D *hist = new TH1D(name.c_str(), name.c_str(), bins, low, high);

	for (int i = 0; i < 8; i++)
	{
		int side = i / 4;
		string ch;
		(side == 0) ? ch = "B" : ch = "T";
		int seg = i % 4 + 1;
		string gate = Form("eneAll> %d && eneAll< %d && ene%s%d>0", min, max, ch.c_str(), seg);
		//   tree->Draw(Form("ene%s%d>>h(%d,%d,%d)",ch.c_str(), seg, bins, low, high), gate.c_str());
		tree->Project(Form("h(%d,%d,%d)", bins, low, high), Form("ene%s%d", ch.c_str(), seg), gate.c_str(), "");
		TH1D *h = (TH1D *)gDirectory->Get("h");
		hist->Add(h);
	}

	return hist;
}

TH1D *make_sos(TTree *tree, int min, int max, string name, int bins, int low, int high)
{

	TH1D *hist = new TH1D(name.c_str(), name.c_str(), bins, low, high);
    std::vector<std::string> chans = {"B2", "B3", "T2", "T3"};

	for (auto ch : chans)
	{
		string gate = Form("eneAll> %d && eneAll< %d && ene%s>0", min, max, ch.c_str());
		tree->Project(Form("h(%d,%d,%d)", bins, low, high), Form("ene%s", ch.c_str()), gate.c_str(), "");
		TH1D *h = (TH1D *)gDirectory->Get("h");
		hist->Add(h);
	}

	return hist;
}

void create_simulation_input_file()
{

	// Directory where simulation ROOT files are stored
	string directory = "../geant_outputs/";

	// Specific parameters
	vector<string> param = {"cu63_Ex_10MeV_0.5p", "cu63_Ex_10MeV_0.5m", "cu63_Ex_10MeV_1.5p", "cu63_Ex_10MeV_1.5m", "cu63_Ex_10MeV_2.5p","cu63_Ex_10MeV_2.5m","cu63_Ex_10MeV_3.5m"};

	// File names
	string ext="_T1.05";
	string fout_name = Form("sim_hsts_Jp%s.root",ext.c_str());

	// Gating conditions and histogram sizes
	int bins = 100;
	int high = 12000;
	int low = 5000;

	int sbins = 500;
	int shigh = 12001;
	int slow = 1;

	int gatemin = 9600;
	int gatemax = 10400;

	string gate = Form("eneAll > %d && eneAll < %d ", gatemin, gatemax);

	vector<TH1D *> histograms_TAS;
	vector<TH1D *> histograms_Seg;
	vector<TH1D *> histograms_Mul;

	// Draw (project) the TAS, sum-of-segments, and multiplicity spectra of all cascades/components and write to file
	int i = 0;
	for (auto p : param)
	{
		std::cout << "i: " << i << std::endl;

		string fin_name = Form("%s%s.root", p.c_str(),ext.c_str());
		std::string file_name = directory + fin_name;
		cout << file_name.c_str() << endl;

		TFile *fIn = new TFile(file_name.c_str(), "READ");
		TTree *t = (TTree *)fIn->Get("t");

		// TAS
		t->Project(Form("c_%d_TAS(%d,%d,%d)", i, bins, low, high), // c = cascade/component
				   "eneAll",
				   "",
				   "");

		histograms_TAS.push_back((TH1D *)gDirectory->Get(Form("c_%d_TAS", i)));
		histograms_TAS.back()->SetDirectory(0);
		std::cout << histograms_TAS.back()->GetName() << "\t"
				  << histograms_TAS.back()->GetTitle() << std::endl;

		// SEG
		histograms_Seg.push_back(new TH1D(Form("c_%d_Seg", i), "", sbins, slow, shigh)); // c = cascade/component, Seg = segments
		histograms_Seg.back()->SetDirectory(0);
		std::cout << histograms_Seg.back()->GetName() << "\t"
				  << histograms_Seg.back()->GetTitle() << std::endl;
		TH1D *sos = make_sos(t, gatemin, gatemax, "sos", sbins, slow, shigh);
		histograms_Seg.back()->Add(sos, +1);

		// MUL
		t->Project(Form("c_%d_Mul(8,1,9)", i), // c = cascade/component, Mul = multiplicity
				   "multi",
				   gate.c_str(),
				   "");
		histograms_Mul.push_back((TH1D *)gDirectory->Get(Form("c_%d_Mul", i)));
		histograms_Mul.back()->SetDirectory(0);
		std::cout << histograms_Mul.back()->GetName() << "\t"
				  << histograms_Mul.back()->GetTitle() << std::endl;

		fIn->Close();
		i++;
	}

	assert(histograms_TAS.size() == histograms_Seg.size());
	assert(histograms_Seg.size() == histograms_Mul.size());

	// Output ROOT file for the histograms
	TFile *fOut = new TFile(fout_name.c_str(), "RECREATE");

	for (const auto &hist : histograms_TAS)
	{
		hist->Write();
	}
	for (const auto &hist : histograms_Seg)
	{
		hist->Write();
	}
	for (const auto &hist : histograms_Mul)
	{
		hist->Write();
	}


	fOut->Close();
}
