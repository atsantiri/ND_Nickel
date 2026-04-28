#include "TH1.h"
#include "TLegend.h"

void test_norm(){

	TCanvas *c1 = new TCanvas("c1","Gamma-ray strength function",600,600);
	TH2F *h = new TH2F("h"," ",10,0.0,  10.356,10,1.262e-09,3.153e-06);
	h->SetStats(false);

	ifstream strengthfile("strength_d70_g500");
	float strength[46],strengtherr[46],energyerr[46];
	float energy[451],trans[451];
	int i = 0;
   float a0 =  -0.7040;
   float a1 =   0.2400;
	float x;	
	while(strengthfile){
		strengthfile >> x;
		if(i<45){
			strength[i] = x;
			energy[i] = a0 + (a1*i);
			energyerr[i] = 0.0;
		}	
		else{strengtherr[i-45] = x;}
		i++;
	}
	TGraphErrors* strengthexp = new TGraphErrors(46,energy,strength,energyerr,strengtherr);
    i = 0;

	ifstream strengthfile2("strength_d170_g500");
	float strength2[46],strengtherr2[46];
	i = 0;
	while(strengthfile2){
		strengthfile2 >> x;
		if(i<45){
			strength2[i] = x;		}	
		else{strengtherr2[i-45] = x;}
		i++;
	}
	TGraphErrors *strengthexp2 = new TGraphErrors(46,energy,strength2,energyerr,strengtherr2);
    i = 0;
    
	ifstream strengthfile3("strength_d170_g1000");
	float strength3[46],strengtherr3[46];
	i = 0;
	while(strengthfile3){
		strengthfile3 >> x;
		if(i<45){
			strength3[i] = x;		}	
		else{strengtherr3[i-45] = x;}
		i++;
	}
	TGraphErrors *strengthexp3 = new TGraphErrors(46,energy,strength3,energyerr,strengtherr3);
    i = 0;   
    	ifstream strengthfile4("strength_d70_g1000");
	float strength4[46],strengtherr4[46];
	i = 0;
	while(strengthfile4){
		strengthfile4 >> x;
		if(i<45){
			strength4[i] = x;		}	
		else{strengtherr4[i-45] = x;}
		i++;
	}
	TGraphErrors *strengthexp4 = new TGraphErrors(46,energy,strength4,energyerr,strengtherr4);

	c1->SetLogy();
	c1->SetLeftMargin(0.14);
	h->GetXaxis()->CenterTitle();
	h->GetXaxis()->SetTitle("#gamma-ray energy E_{#gamma} (MeV)");
	h->GetYaxis()->CenterTitle();
	h->GetYaxis()->SetTitleOffset(1.4);
	h->GetYaxis()->SetTitle("#gamma-ray strength function (MeV^{-3})");
	h->Draw();
	strengthexp2->SetMarkerStyle(21);
	strengthexp2->SetMarkerSize(0.8);
	strengthexp2->Draw("P SAME");
	strengthexp3->SetMarkerStyle(21);
	strengthexp3->SetMarkerSize(0.8);
	strengthexp3->SetMarkerColor(3);
	strengthexp3->Draw("P SAME");
	strengthexp4->SetMarkerStyle(21);
	strengthexp4->SetMarkerSize(0.8);
	strengthexp4->SetMarkerColor(4);
	strengthexp4->Draw("P SAME");
	strengthexp->SetMarkerStyle(21);
	strengthexp->SetMarkerSize(0.8);
	strengthexp->SetMarkerColor(2);
	strengthexp->Draw("P SAME");
//	strengthexp2->Draw("same");
//    rsfext->SetLineColor(4);
 //   rsfext->SetLineWidth(2);
 //   rsfext->Draw("L");
    	auto* legend = new TLegend(0.6,0.8,0.9,0.9);
	legend->SetTextFont(132);
	legend->AddEntry(strengthexp,"D=70, G=500","ep");
	legend->AddEntry(strengthexp2,"D=170, G=500","ep");
	legend->AddEntry(strengthexp3,"D=170, G=1000","ep");
	legend->AddEntry(strengthexp4,"D=70, G=1000","ep");
	legend->Draw();
    
	TLatex t;
	t.SetTextSize(0.05);
	t.DrawLatex(    1.971,9.460e-07,"^{61}Cu");
	c1->Update();
//	c1->Print("strength.pdf");
//	c1->Print("strength.eps");
//	c1->Print("strength.ps");
}
