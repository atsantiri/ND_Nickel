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
   TH2F *h = new TH2F("h"," ",10,-0.784000,  13.656,50,3.227e-01,5.636e+07);
   ifstream sigfile("sigpaw.cnt");
   float sig[86],sigerr[86];
   float energy[126],energyerr[126];
   float extL[127],extH[127];
   int i;
   float a0 = -0.7840;
   float a1 =  0.1600;
   for(i = 0; i < 85; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<85){
   		sig[i]=x;
   	}
   	else{sigerr[i-85]=x;}
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
   TGraph *extLgraph = new TGraph(85,energy,extL);
   TGraph *extHgraph = new TGraph(85,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(85,energy,sig,energyerr,sigerr);
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
   extLgraph->DrawGraph(25,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(16,&extHgraph->GetX()[69],&extHgraph->GetY()[69],"L");
   TArrow *arrow1 = new TArrow(2.096e+00,1.843e+02,2.096e+00,3.093e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.056e+00,3.701e+02,3.056e+00,6.212e+01,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(1.026e+01,7.565e+05,1.026e+01,1.270e+05,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(1.202e+01,1.145e+07,1.202e+01,1.922e+06,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
