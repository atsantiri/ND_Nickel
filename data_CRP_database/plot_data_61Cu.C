{
	TGraphErrors *gsf1 = new TGraphErrors("f1_exp_029_061_PG_1982NE06.dat","%lg %lg %lg %lg");
	TGraphErrors *gsf2 = new TGraphErrors("f1_exp_029_061_PG_1982NI05.dat","%lg %lg %lg %lg");
	
	gROOT->Reset();
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetFillColor(0);
	gStyle->SetPadBorderMode(0);
	m = (TH1F*)gROOT->FindObject("h");
	if (m) m->Delete();
	TCanvas *c1 = new TCanvas("c1","Gamma-ray strength function",600,600);
	TH2F *h = new TH2F("h"," ",10,0.0,  10.916,10,1e-14,1.745e-06);
	c1->SetLogy();
	c1->SetLeftMargin(0.14);
	h->GetXaxis()->CenterTitle();
	h->GetXaxis()->SetTitle("#gamma-ray energy E_{#gamma} (MeV)");
	h->GetYaxis()->CenterTitle();
	h->GetYaxis()->SetTitleOffset(1.4);
	h->GetYaxis()->SetTitle("#gamma-ray strength function (MeV^{-3})");
	h->Draw();
	
	gsf1->Draw("AP");
	gsf2->Draw("same");
	
	TLatex t;
	t.SetTextSize(0.05);
	t.DrawLatex(2.083,5.235e-07,"^{61}Cu");
	c1->Update();



}
