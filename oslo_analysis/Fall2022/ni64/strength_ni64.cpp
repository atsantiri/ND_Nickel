{
	TGraphAsymmErrors *gsf1 = new TGraphAsymmErrors("../../data_CRP_database/f1_exp_029_065_PG_1979ER10.dat","%lg %lg %lg %lg");
	TGraphErrors *gsf2 = new TGraphErrors("../../data_CRP_database/f1_exp_029_065_PG_1983NE03.dat","%lg %lg %lg %lg");
	
	gROOT->Reset();
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetFillColor(0);
	gStyle->SetPadBorderMode(0);
	m = (TH1F*)gROOT->FindObject("h");
	if (m) m->Delete();
	TCanvas *c1 = new TCanvas("c1","Gamma-ray strength function",600,600);
	TH2F *h = new TH2F("h"," ",10,0.0,  12.216,10,9.237e-10,1.816e-05);
	ifstream strengthfile("un80keV/strength.nrm");
	float strength[80],strengtherr[80],energyerr[80];
	float energy[791],trans[791];
	int i = 0;
   float a0 =  -0.7640;
   float a1 =   0.1600;
	float x;	
	while(strengthfile){
		strengthfile >> x;
		if(i<79){
			strength[i] = x;
			energy[i] = a0 + (a1*i);
			energyerr[i] = 0.0;
		}	
		else{strengtherr[i-79] = x;}
		i++;
	}
	TGraphErrors *strengthexp = new TGraphErrors(80,energy,strength,energyerr,strengtherr);
    i = 0;

    ifstream transfile("un80keV/transext.nrm");
    while(transfile){
        transfile >> x;
        energy[i] = a0 + (a1*i);
        trans[i] = x/(2.*3.14*energy[i]*energy[i]*energy[i]);
        i++;
    }
    TGraph *rsfext = new TGraph(i,energy,trans);
	c1->SetLogy();
	c1->SetLeftMargin(0.14);
	h->GetXaxis()->CenterTitle();
	h->GetXaxis()->SetTitle("#gamma-ray energy E_{#gamma} (MeV)");
	h->GetYaxis()->CenterTitle();
	h->GetYaxis()->SetTitleOffset(1.4);
	h->GetYaxis()->SetTitle("#gamma-ray strength function (MeV^{-3})");
	h->Draw();
	strengthexp->SetMarkerStyle(21);
	strengthexp->SetMarkerSize(0.8);
	
		
////////////////////////////////////////////////////////////////////////////////////////////
	levGraph   = new TGraphErrors();
	TRandom3 *r = new TRandom3(0);
	std::ifstream inp;
	inp.open("../../talys/gSF_band_cu65_oslo.txt");
	if (inp.is_open() ) {
        
		double e_gamma;
		double oslo_gSF_high, oslo_gSF_low, oslo_gSF, oslo_dgSF;
        
		while ( !inp.eof() ) {
			inp >> e_gamma >> oslo_gSF_high >>oslo_gSF_low;
        	    
			oslo_gSF = ( oslo_gSF_high + oslo_gSF_low ) / 2;
			oslo_dgSF =( oslo_gSF_high - oslo_gSF_low ) / 2;
        	    
        	   
			levGraph->SetPoint(levGraph->GetN(), e_gamma, oslo_gSF);
			levGraph->SetPointError(levGraph->GetN() - 1, 0, oslo_dgSF);
            
        	}
        
        //apply transformation according to the settings file
        //Transform();
        
        	levGraph->SetFillColor(4);
        	levGraph->SetFillStyle(3010);
        	levGraph->Draw("same");
	}
	
	TGraph *talysmax = new TGraph("../../talys/gSF_band_cu65_oslo.txt","%lg %lg %*lg");
	TGraph *talysmin = new TGraph("../../talys/gSF_band_cu65_oslo.txt","%lg %*lg %lg");
	talysmax->SetFillColorAlpha(94,.4);
	talysmax->Draw("same c");
	talysmin->SetFillColorAlpha(94,.4);
	talysmin->Draw("same c");
	TGraph *shade = new TGraph("../../talys/gSF_band_cu65_oslo_rev", "%lg %lg");
	shade->SetFillStyle(3010);
	shade->SetFillColor(4);
	shade->Draw("F");
/////////////////////////////////////////////////////////////////////////////////////////////

	
	strengthexp->Draw("P");
	
	gsf1->SetMarkerStyle(21);
	gsf1->SetMarkerSize(0.8);
	gsf1->SetMarkerColor(2);
	gsf1->Draw("P SAME");
	gsf2->SetMarkerStyle(21);
	gsf2->SetMarkerSize(0.8);
	gsf2->SetMarkerColor(3);
	gsf2->Draw("P SAME");
	
	   auto* legend = new TLegend(0.6,0.8,0.9,0.9);
	legend->SetTextFont(132);
	legend->AddEntry(strengthexp,"this","ep");
	legend->AddEntry(gsf1,"Erlandsson (1979) p,g exp","ep");
	legend->AddEntry(gsf2,"Nemashkalo (1983) p,g exp","ep");
	legend->Draw();
	
    rsfext->SetLineColor(4);
    rsfext->SetLineWidth(2);
    rsfext->Draw("L");
	TLatex t;
	t.SetTextSize(0.05);
	t.DrawLatex(    2.439,8.071e-06,"^{65}Cu");
	c1->Update();
	c1->Print("strength_Ni64.png");
//	c1->Print("strength.eps");
//	c1->Print("strength.ps");
}
