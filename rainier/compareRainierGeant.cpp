#include "ROOT/RDataFrame.hxx"
#include "TString.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"

TH1D *make_sos_df(ROOT::RDataFrame &d, int min, int max, TString name, int bins, double low, double high)
{
    TH1D *hist = new TH1D(name, name, bins, low, high);
    auto df = d.Filter(Form("eneAll > %d && eneAll < %d && multi ==2", min, max));
    std::vector<std::string> chans = {"B2", "B3", "T2", "T3"};

    for (auto &ch : chans)
    {
        auto htemp = df.Filter(Form("ene%s > 0", ch.c_str())).Histo1D({("h" + ch).c_str(), "", bins, low, high}, Form("ene%s", ch.c_str()));
        hist->Add((TH1F *)htemp->Clone());
    }

    return hist;
}

double normToInt(TH1D *h1, TH1D *h2, int min, int max)
{
    double ret{0};
    int b1 = h1->GetXaxis()->FindBin(min);
    int b2 = h1->GetXaxis()->FindBin(max);

    double i1 = h1->Integral(b1, b2);
    double i2 = h2->Integral(b1, b2);

    if (i1 > 0)
        ret = i1 / i2;
    else
        std::cout << "integral is 0" << std::endl;
    return ret;
}

void compareRainierGeant()
{
    // TString rainIn = "geant_outputs/cu63_Ex_10keV.root";
    std::vector<TString> files = {"cu63_Ex_10MeV_2.5p","cu63_Ex_10MeV_2.5p_noLEE"};
    // std::vector<TString> files = {"cu63_Ex_10MeV_0.5p","cu63_Ex_10MeV_2.5p","cu63_Ex_10MeV_0.5p_T1.0","cu63_Ex_10MeV_2.5p_T1.0"};
    // std::vector<TString> files = {"cu63_Ex_10MeV_0.5p", "cu63_Ex_10MeV_0.5m", "cu63_Ex_10MeV_1.5p","cu63_Ex_10MeV_1.5m", "cu63_Ex_10MeV_2.5p"};

    TString dataIn = "../rootfiles_raw/runfiles/run_1205_thres_040_keV.root";
    ROOT::RDataFrame d("ttc", dataIn);
    auto hdata = d.Histo1D({"heneAll", "eneAll;E [keV];Counts", 500, 5000, 12001}, "eneAll");
    TCanvas *c0 = new TCanvas("c0", "TAS", 1200, 800);
    hdata->DrawClone();
    TCanvas *c1 = new TCanvas("c1", "SoS", 1200, 800);
    c1->cd();
    gPad->SetLogy();
    auto hsosdata = make_sos_df(d, 9600, 10400, "hSoS (9600-10400)", 500, 0, 12000);
    hsosdata->Draw();
    int i = 0;
    std::vector<int> cols = {2,3,7,6,25,46};
    auto l = new TLegend(0.7, 0.7, 0.9, 0.9);
    l->AddEntry(hsosdata, "exp");
    TCanvas *c2 = new TCanvas("c2", "Mul", 1000, 800);
    c2->cd();
    auto hmuldata = d.Histo1D({"hmul","mul;mul;counts",7,1,8},"multi");
    hmuldata->DrawClone();

    for (auto s : files)
    {
        TString rainIn = TString::Format("geant_outputs/%s.root", s.Data());
        ROOT::RDataFrame dr("t", rainIn);

        auto hsim = dr.Histo1D({TString::Format("heneAllsim%d",i), "eneAll;E [keV];Counts", 500, 5000, 12001}, "eneAll");

        hsim->SetLineColor(cols[i]);
        double scale = hdata->GetMaximum() / hsim->GetMaximum();
        hsim->Scale(scale);
        c0->cd();
        hsim->DrawClone("same hist");
        auto hsosSim = make_sos_df(dr, 9600, 10400, TString::Format("hsissim%d",i), 500, 0, 12000);
        c1->cd();
        hsosSim->SetLineColor(cols[i]);
        hsosSim->Scale(normToInt(hsosdata, hsosSim, 0, 12000));
        // if (i==4)
            // hsosSim->SetLineWidth(2);
        hsosSim->Draw("same hist");
        i++;
        l->AddEntry(hsosSim, s);
        c2->cd();
        auto hmulsim = dr.Histo1D({TString::Format("hmulsim%d",i),"mul;mul;counts",7,1,8},"multi");
        hmulsim->SetLineColor(cols[i]);
        hmulsim->Scale(normToInt(hmuldata.GetPtr(), hmulsim.GetPtr(),1,8));
        hmulsim->DrawClone("same hist");
        
    }
    c1->cd();
    l->Draw();
}