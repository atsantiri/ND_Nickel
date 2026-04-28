#include "TH1.h"

void decomposition(){

/*	double Eeff[3]  = {2.3839,2.6842,2.9888};
	double cs[3]    = {0.23469,0.72474,1.63556};
	double cs_err[3]= {0.04355, 0.1583, 0.39644};
	double e_err_h[3]={0.0242, 0.0276, 0.0299};
	double e_err_l[3]={0.0900, 0.0570, 0.0624};
*/
	TCanvas *c = new TCanvas("c","Ni58 cross section");
	c->SetLogy();

	TH2F *h1 = new TH2F("h1","",10,1,10,500,5e-3,1000);
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

	TGraph *data = new TGraph("simon_58ni","%lg %lg");
	data->SetMarkerStyle(8);
	data->SetMarkerColor(1);
	data->SetLineColor(1);

	

	TGraph *pg = new TGraph("rp029059.tot","%lg %lg");
	pg->SetLineColor(2);
	pg->SetLineWidth(2);
	pg->Draw("same");
	TGraph *pn = new TGraph("rp029058.tot","%lg %lg");
	pn->SetLineColor(4);
	pn->SetLineWidth(2);
	pn->Draw("same");
	TGraph *pp = new TGraph("rp028058.tot","%lg %lg");
	pp->SetLineColor(65);
	pp->SetLineWidth(2);
	pp->Draw("same");
	TGraph *pa = new TGraph("rp027055.tot","%lg %lg");
	pa->SetLineColor(8);
	pa->SetLineWidth(2);
	pa->Draw("same");
	
	TGraph *comp_el = new TGraph("total.tot","%lg %*lg %*lg %*lg %lg");
	comp_el->SetLineColor(6);
	comp_el->SetLineWidth(2);
	comp_el->Draw("same");

	TGraph *tot = new TGraph("total.tot","%lg %*lg %*lg %*lg %*lg %*lg %lg");
	tot->SetLineColor(93);
	tot->SetLineWidth(2);
	tot->SetLineStyle(2);
	tot->Draw("same");
			

	data->Draw("same PE");

//	TGraph *nonsmoker = new TGraph("nonsmoker","%lg %lg");
//	nonsmoker->SetLineColor(4);
//	nonsmoker->SetLineWidth(2);
//	nonsmoker->Draw("same c");

	TGraph *nonsmoker = new TGraph("non-smoker2","%lg %lg");
	nonsmoker->SetLineColor(2);
	nonsmoker->SetLineWidth(2);
	nonsmoker->SetLineStyle(2);
	nonsmoker->Draw("same c");
	
	TGraph *nonsmoker_pn = new TGraph("non-smoker2","%lg %*lg %lg");
	nonsmoker_pn->SetLineColor(4);
	nonsmoker_pn->SetLineWidth(2);
	nonsmoker_pn->SetLineStyle(2);
	nonsmoker_pn->Draw("same c");

	TGraph *nonsmoker_pa = new TGraph("non-smoker2","%lg %*lg %*lg %lg");
	nonsmoker_pa->SetLineColor(8);
	nonsmoker_pa->SetLineWidth(2);
	nonsmoker_pa->SetLineStyle(2);
	nonsmoker_pa->Draw("same c");

	auto* legend = new TLegend(0.1,0.6,0.4,0.9);
	//legend->SetHeader("","C");
	//legend->AddEntry((TObject*)0, "", "");
	legend->SetTextFont(132);
//	legend->SetHeader("^{82}Kr(p,#gamma)^{83}Rb Cross Section","C");
	legend->AddEntry(data,"Simon 2018");
	legend->AddEntry(pg,"p,g (59Cu)");
	legend->AddEntry(nonsmoker,"non smoker p,g");
	legend->AddEntry(pn,"p,n (58Cu)");
	legend->AddEntry(nonsmoker_pn,"non smoker p,n");
	legend->AddEntry(pp,"p,p' (58Ni)");
	legend->AddEntry(pa,"p,a (55Co)");
	legend->AddEntry(nonsmoker_pa,"non smoker p,a");	
	legend->AddEntry(comp_el,"Compound Elastic p,p0 58Ni(g,s)");
	legend->AddEntry(tot,"Total");
	legend->Draw();

//	c->SaveAs("cs_decomp.pdf");

}
