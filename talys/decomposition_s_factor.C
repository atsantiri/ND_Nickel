#include "TH1.h"

void decomposition_s_factor(){
/*
	double Eeff[3]  = {2.3839,2.6842,2.9888};
	double s[3]    = {5.56835446e+09, 5.14147876e+09, 4.15178478e+09};
//	double s[3]    = {2261819.39605822, 4876294.57732573 ,8120109.76893209};

	double s_err[3]= {1.03328577e+09, 1.12301803e+09, 1.00634251e+09};
//	double s_err[3]= {420249.30542402, 1066007.61143272, 1973598.3794957};
	double e_err_h[3]={0.0242, 0.0276, 0.0299};
	double e_err_l[3]={0.0900, 0.0570, 0.0624};
*/
	TCanvas *c = new TCanvas("c","Ni58 cross section");
	c->SetLogy();

	TH2F *h1 = new TH2F("h1","",10,1,10,500,1e3,1e10);
	h1->SetStats(false);
	h1->GetXaxis()->SetTitle("Energy (MeV)");
	h1->GetXaxis()->SetTitleOffset(0.95);
	h1->GetXaxis()->SetTitleFont(132);
	h1->GetXaxis()->CenterTitle(true);
	h1->GetYaxis()->SetTitleOffset(0.95);
	h1->GetYaxis()->CenterTitle(true);
	h1->GetYaxis()->SetTitle("S(E) (MeV * mb)");
	h1->GetYaxis()->SetTitleFont(132);
	h1->GetXaxis()->SetTitleSize(.048);
	h1->GetYaxis()->SetTitleSize(.048);
	h1->Draw();

//	TGraphAsymmErrors *data = new TGraphAsymmErrors(3,Eeff,s,e_err_l,e_err_h,s_err,s_err);

//	data->SetMarkerStyle(8);
//	data->SetMarkerColor(1);
//	data->SetLineColor(1);

	
	
	TGraph *pg = new TGraph("rp029059_s.tot","%lg %lg");
	pg->SetLineColor(2);
	pg->SetLineWidth(2);
	pg->Draw("same");
	TGraph *pn = new TGraph("rp029058_s.tot","%lg %lg");
	pn->SetLineColor(4);
	pn->SetLineWidth(2);
	pn->Draw("same");
	TGraph *pp = new TGraph("rp028058_s.tot","%lg %lg");
	pp->SetLineColor(65);
	pp->SetLineWidth(2);
	pp->Draw("same");
	TGraph *pa = new TGraph("rp027055_s.tot","%lg %lg");
	pa->SetLineColor(8);
	pa->SetLineWidth(2);
	pa->Draw("same");
	
	TGraph *comp_el = new TGraph("total_s.tot","%lg %lg %*lg");
	comp_el->SetLineColor(6);
	comp_el->SetLineWidth(2);
	comp_el->Draw("same");

	TGraph *tot = new TGraph("total_s.tot","%lg %*lg %lg");
	tot->SetLineColor(93);
	tot->SetLineWidth(2);
	tot->SetLineStyle(2);
	tot->Draw("same");
			

//	data->Draw("same PE");

//	TGraph *nonsmoker = new TGraph("nonsmoker","%lg %lg");
//	nonsmoker->SetLineColor(4);
//	nonsmoker->SetLineWidth(2);
//	nonsmoker->Draw("same c");


	auto* legend = new TLegend(0.6,0.65,0.9,0.9);
	//legend->SetHeader("","C");
	//legend->AddEntry((TObject*)0, "", "");
	legend->SetTextFont(132);
	legend->SetTextSize(0.035);
//	legend->SetHeader("^{82}Kr(p,#gamma)^{83}Rb Cross Section","C");
//	legend->AddEntry(data,"Present Work");
	legend->AddEntry(pg,"^{59}Cu production (p,#gamma)");
	legend->AddEntry(pn,"^{58}Cu production (p,n)");
	legend->AddEntry(pp,"^{58}Ni production (p,p')");
	legend->AddEntry(pa,"^{55}Co production (p,#alpha)");
	legend->AddEntry(comp_el,"Compound Elastic");
	legend->AddEntry(tot,"Total");
	legend->Draw();

	c->SaveAs("s_factor_decomp.pdf");

}
