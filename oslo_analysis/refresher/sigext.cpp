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
   TH2F *h = new TH2F("h"," ",10,-0.816500,  11.464,50,9.082e-01,1.075e+05);
   ifstream sigfile("sigpaw.cnt");
   float sig[96],sigerr[96];
   float energy[167],energyerr[167];
   float extL[168],extH[168];
   int i;
   float a0 = -0.8165;
   float a1 =  0.1200;
   for(i = 0; i < 102; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<95){
   		sig[i]=x;
   	}
   	else{sigerr[i-95]=x;}
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
   TGraph *extLgraph = new TGraph(102,energy,extL);
   TGraph *extHgraph = new TGraph(102,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(95,energy,sig,energyerr,sigerr);
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
   extLgraph->DrawGraph(51,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(42,&extHgraph->GetX()[60],&extHgraph->GetY()[60],"L");
   TArrow *arrow1 = new TArrow(3.984e+00,1.947e+02,3.984e+00,6.114e+01,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(5.183e+00,6.363e+02,5.183e+00,1.998e+02,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(6.384e+00,2.361e+03,6.384e+00,7.413e+02,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(7.583e+00,8.213e+03,7.583e+00,2.578e+03,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
