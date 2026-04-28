{
   gROOT->Reset();
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetFillColor(0);
   gStyle->SetPadBorderMode(0);
   m = (TH1F*)gROOT->FindObject("h");
   if (m) m->Delete();
   TCanvas *c1 = new TCanvas("c1","Normalization of gamma-transmission coefficient",600,600);
   TH2F *h = new TH2F("h"," ",10,-0.704000,  10.856,50,4.478e-01,6.816e+04);
   ifstream sigfile("sigpaw.cnt");
   float sig[46],sigerr[46];
   float energy[84],energyerr[84];
   float extL[85],extH[85];
   int i;
   float a0 = -0.7040;
   float a1 =  0.2400;
   for(i = 0; i < 55; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<45){
   		sig[i]=x;
   	}
   	else{sigerr[i-45]=x;}
   	i++;
   }
   ifstream extendfile("extendLH.cnt");
   i = 0;
   while(extendfile){
   	extendfile >> x >> y ;
   	extL[i]=x;
   	extH[i]=y;
   	i++;
   }
   TGraph *extLgraph = new TGraph(55,energy,extL);
   TGraph *extHgraph = new TGraph(55,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(45,energy,sig,energyerr,sigerr);
   c1->SetLogy();
   c1->SetLeftMargin(0.14);
   h->GetXaxis()->CenterTitle();
   h->GetXaxis()->SetTitle("#gamma-ray energy E_{#gamma} (MeV)");
   h->GetYaxis()->CenterTitle();
   h->GetYaxis()->SetTitleOffset(1.4);
   h->GetYaxis()->SetTitle("Transmission coeff. (arb. units)");
   h->Draw();
   sigexp->SetMarkerStyle(21);
   sigexp->SetMarkerSize(0.8);
   sigexp->Draw("P");
   extLgraph->SetLineStyle(1);
   extLgraph->DrawGraph(16,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(35,&extHgraph->GetX()[20],&extHgraph->GetY()[20],"L");
   TArrow *arrow1 = new TArrow(2.176e+00,4.148e+01,2.176e+00,1.363e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(2.896e+00,7.754e+01,2.896e+00,2.548e+01,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(4.096e+00,1.398e+02,4.096e+00,4.594e+01,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(8.176e+00,6.313e+03,8.176e+00,2.074e+03,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
