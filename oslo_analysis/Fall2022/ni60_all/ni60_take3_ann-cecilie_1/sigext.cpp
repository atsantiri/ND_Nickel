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
   TH2F *h = new TH2F("h"," ",10,-0.824000,  11.096,50,4.732e-01,6.800e+04);
   ifstream sigfile("sigpaw.cnt");
   float sig[93],sigerr[93];
   float energy[167],energyerr[167];
   float extL[168],extH[168];
   int i;
   float a0 = -0.8240;
   float a1 =  0.1200;
   for(i = 0; i < 110; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<92){
   		sig[i]=x;
   	}
   	else{sigerr[i-92]=x;}
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
   TGraph *extLgraph = new TGraph(110,energy,extL);
   TGraph *extHgraph = new TGraph(110,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(92,energy,sig,energyerr,sigerr);
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
   extLgraph->DrawGraph(35,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(50,&extHgraph->GetX()[60],&extHgraph->GetY()[60],"L");
   TArrow *arrow1 = new TArrow(2.296e+00,4.978e+01,2.296e+00,1.636e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.256e+00,1.003e+02,3.256e+00,3.297e+01,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(6.376e+00,1.364e+03,6.376e+00,4.484e+02,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(9.736e+00,2.011e+04,9.736e+00,6.610e+03,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
