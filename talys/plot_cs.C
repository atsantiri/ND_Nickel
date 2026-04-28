#include "TH1.h"

void plot_cs(){

	double Eeff[3]  = {2.3839,2.6842,2.9888};
	double cs[3]    = {0.23469,0.72474,1.63556};
	double cs_err[3]= {0.04355, 0.1583, 0.39644};
	double e_err_h[3]={0.0242, 0.0276, 0.0299};
	double e_err_l[3]={0.0900, 0.0570, 0.0624};

	TCanvas *c = new TCanvas("c","Kr82 cross section");
	c->SetLogy();

	TH2F *h1 = new TH2F("h1","",10,1.5,4,500,5e-2,10);
	h1->SetStats(false);
	h1->GetXaxis()->SetTitle("Center of Mass Energy (MeV)");
	h1->GetXaxis()->SetTitleOffset(0.95);
	h1->GetXaxis()->SetTitleFont(132);
	h1->GetXaxis()->CenterTitle(true);
	h1->GetYaxis()->SetTitleOffset(0.95);
	h1->GetYaxis()->CenterTitle(true);
	h1->GetYaxis()->SetTitle("Cross Section (mb)");
	h1->GetYaxis()->SetTitleFont(132);
	h1->GetXaxis()->SetTitleSize(.048);
	h1->GetYaxis()->SetTitleSize(.048);
	h1->Draw();

//	TGraphErrors *data = new TGraphErrors(3,Eeff,cs,nullptr,cs_err);
	TGraphAsymmErrors *data = new TGraphAsymmErrors(3,Eeff,cs,e_err_l,e_err_h,cs_err,cs_err);

	data->SetMarkerStyle(8);
	data->SetMarkerColor(1);
	data->SetLineColor(1);


	TGraph *talysmax = new TGraph("max","%lg %lg");
	talysmax->SetFillColorAlpha(93,.8);
	//talysmax->Draw("same c");
	TGraph *talysmin = new TGraph("min","%lg %lg");
	talysmin->SetFillColorAlpha(93,.8);
	//talysmin->Draw("same c");
	TGraph *shade = new TGraph("talys_shade", "%lg %lg");
	shade->SetFillStyle(1001);
	shade->SetFillColorAlpha(93,0.8);
	shade->Draw("f");

	data->Draw("same PE");

	TGraph *nonsmoker = new TGraph("nonsmoker","%lg %lg");
	nonsmoker->SetLineColor(4);
	nonsmoker->SetLineWidth(2);
	nonsmoker->Draw("same c");


	auto* legend = new TLegend(0.1,0.7,0.4,0.9);
	//legend->SetHeader("","C");
	//legend->AddEntry((TObject*)0, "", "");
	legend->SetTextFont(132);
//	legend->SetHeader("^{82}Kr(p,#gamma)^{83}Rb Cross Section","C");
	legend->AddEntry(data,"Present Work");
	legend->AddEntry(nonsmoker,"NON-SMOKER");
	legend->AddEntry(talysmax,"TALYS");
	legend->Draw();

//	c->SaveAs("~/My_Documents/82Kr_pg/poster_plots/cs.pdf");

}
