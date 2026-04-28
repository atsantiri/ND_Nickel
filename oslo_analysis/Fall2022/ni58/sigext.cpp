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
   TH2F *h = new TH2F("h"," ",10,-0.884000,   9.716,50,1.299e-01,1.964e+04);
   ifstream sigfile("sigpaw.cnt");
   float sig[32],sigerr[32];
   float energy[63],energyerr[63];
   float extL[64],extH[64];
   int i;
   float a0 = -0.8840;
   float a1 =  0.3200;
   for(i = 0; i < 45; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<31){
   		sig[i]=x;
   	}
   	else{sigerr[i-31]=x;}
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
   TGraph *extLgraph = new TGraph(45,energy,extL);
   TGraph *extHgraph = new TGraph(45,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(31,energy,sig,energyerr,sigerr);
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
   extLgraph->DrawGraph(15,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(20,&extHgraph->GetX()[25],&extHgraph->GetY()[25],"L");
   TArrow *arrow1 = new TArrow(2.316e+00,9.379e+00,2.316e+00,3.490e+00,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.596e+00,2.478e+01,3.596e+00,9.222e+00,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(7.116e+00,1.418e+03,7.116e+00,5.277e+02,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(8.396e+00,6.484e+03,8.396e+00,2.413e+03,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
