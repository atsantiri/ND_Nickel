#include "TFile.h"
#include "ROOT/RDataFrame.hxx"
#include "TH1D.h"

TH1D *make_sos_df(ROOT::RDataFrame &d, int min, int max, TString name, int bins, double low, double high)
{
    TH1D *hist = new TH1D(name, name, bins, low, high);
    auto df = d.Filter(Form("eneAll > %d && eneAll < %d", min, max));
    std::vector<std::string> chans = {"B2", "B3", "T2", "T3"};

    for (auto &ch : chans)
    {
        auto htemp = df.Filter(Form("ene%s > 0", ch.c_str())).Histo1D({("h" + ch).c_str(), "", bins, low, high}, Form("ene%s", ch.c_str()));
        hist->Add((TH1F *)htemp->Clone());
    }

    return hist;
}

void make_exp_chi2_input()
{

    int run = 1205;
    int mingate = 9600;
    int maxgate = 10400;

    TString dataIn = TString::Format("../../rootfiles_raw/runfiles/run_%d_thres_040_keV.root", run);
    ROOT::RDataFrame d("ttc", dataIn);

    // TAS
    auto hdata = d.Histo1D({"data_TAS", "eneAll;E [keV];Counts", 100, 5000, 12000}, "eneAll");
    // SoS
    auto hsosdata = make_sos_df(d, mingate, maxgate, "data_Seg", 500, 1, 12001);
    // Mul
    auto hmuldata = d.Filter(Form("eneAll > %d && eneAll < %d", mingate, maxgate)).Histo1D({"data_Mul", "mul;mul;counts", 8, 1, 9}, "multi");

    TString fout_name = TString::Format("exp_input_run_%d.root", run);
    TFile *fOut = new TFile(fout_name, "RECREATE");
    hdata.GetPtr()->Write();
    hsosdata->Write();
    hmuldata.GetPtr()->Write();
    fOut->Close();
}