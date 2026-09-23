#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"

void draw_comparison()
{
    TFile *fIn = new TFile("comparison_multi2.root", "READ");

    std::vector<std::string> ext = {"_Tdef", "_T10", "_T105"};
    // std::vector<std::string> ext = {"_Tdef", "_noLEE"};
    TCanvas *c0 = new TCanvas("c0", "TAS", 800, 600);
    auto dataTAS = (TH1D *)fIn->Get("data_TAS");
    dataTAS->Draw();

    TCanvas *c1 = new TCanvas("c1", "SoS", 1200, 800);
    c1->cd();
    gPad->SetLogy();
    auto dataSeg = (TH1D *)fIn->Get("data_Seg");
    dataSeg->Draw();
    auto l = new TLegend(0.7, 0.7, 0.9, 0.9);
    l->AddEntry(dataSeg, "exp");

    TCanvas *c2 = new TCanvas("c2", "Mul", 800, 600);
    c2->cd();
    auto dataMul = (TH1D *)fIn->Get("data_Mul");
    dataMul->Draw();

    int i = 0;
    std::vector<int> cols = {4, 6, 95, 3, 25, 46};
    for (auto e : ext)
    {
        c0->cd();
        auto simTAS = (TH1D *)fIn->Get(Form("sim_TAS%s", e.c_str()));
        simTAS->SetLineColor(cols[i]);
        simTAS->Draw("same");

        c1->cd();
        auto simSeg = (TH1D *)fIn->Get(Form("sim_Seg%s", e.c_str()));
        simSeg->SetLineColor(cols[i]);
        simSeg->Draw("same");

        c2->cd();
        auto simMul = (TH1D *)fIn->Get(Form("sim_Mul%s", e.c_str()));
        simMul->SetLineColor(cols[i]);
        simMul->Draw("same");

        l->AddEntry(simSeg, e.c_str());

        i++;
    }
    c1->cd();
    l->Draw();
}