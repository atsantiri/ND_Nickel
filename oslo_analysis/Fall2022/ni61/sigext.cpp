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
   TH2F *h = new TH2F("h"," ",10,-0.784000,  10.296,50,7.735e-01,1.044e+06);
   ifstream sigfile("sigpaw.cnt");
   float sig[65],sigerr[65];
   float energy[126],energyerr[126];
   float extL[127],extH[127];
   int i;
   float a0 = -0.7840;
   float a1 =  0.1600;
   for(i = 0; i < 65; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<64){
   		sig[i]=x;
   	}
   	else{sigerr[i-64]=x;}
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
   TGraph *extLgraph = new TGraph(65,energy,extL);
   TGraph *extHgraph = new TGraph(65,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(64,energy,sig,energyerr,sigerr);
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
   extLgraph->DrawGraph(26,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(23,&extHgraph->GetX()[42],&extHgraph->GetY()[42],"L");
   TArrow *arrow1 = new TArrow(1.616e+00,1.385e+02,1.616e+00,3.463e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.216e+00,3.144e+02,3.216e+00,7.863e+01,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(5.936e+00,3.133e+03,5.936e+00,7.836e+02,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(8.176e+00,4.514e+04,8.176e+00,1.129e+04,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
