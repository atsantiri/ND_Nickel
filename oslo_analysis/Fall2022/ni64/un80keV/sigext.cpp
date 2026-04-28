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
   TH2F *h = new TH2F("h"," ",10,-0.764000,  12.716,50,2.424e-01,4.115e+06);
   ifstream sigfile("sigpaw.cnt");
   float sig[80],sigerr[80];
   float energy[126],energyerr[126];
   float extL[127],extH[127];
   int i;
   float a0 = -0.7640;
   float a1 =  0.1600;
   for(i = 0; i < 79; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<79){
   		sig[i]=x;
   	}
   	else{sigerr[i-79]=x;}
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
   TGraph *extLgraph = new TGraph(79,energy,extL);
   TGraph *extHgraph = new TGraph(79,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(79,energy,sig,energyerr,sigerr);
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
   extHgraph->DrawGraph(19,&extHgraph->GetX()[60],&extHgraph->GetY()[60],"L");
   TArrow *arrow1 = new TArrow(2.116e+00,7.660e+01,2.116e+00,1.670e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.076e+00,1.492e+02,3.076e+00,3.254e+01,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(8.836e+00,4.220e+04,8.836e+00,9.202e+03,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(1.124e+01,8.655e+05,1.124e+01,1.887e+05,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
