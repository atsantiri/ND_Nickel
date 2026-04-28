{

   TGraph *ld1 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %lg %*lg %*lg %*lg %*lg %*lg");	
   TGraph *ld2 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %*lg %lg %*lg %*lg %*lg %*lg");	
   TGraph *ld3 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %*lg %*lg %lg %*lg %*lg %*lg");	
   TGraph *ld4 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %*lg %*lg %*lg %lg %*lg %*lg");	
   TGraph *ld5 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %*lg %*lg %*lg %*lg %lg %*lg");	
   TGraph *ld6 = new TGraph("../../talys/ld_allparity_all_cu61","%lg %*lg %*lg %*lg %*lg %*lg %lg");	

   gROOT->Reset();
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetFillColor(0);
   gStyle->SetPadBorderMode(0);
   m = (TH1F*)gROOT->FindObject("h");
   if (m) m->Delete();
   TCanvas *c1 = new TCanvas("c1","Normalization of level density",600,600);
   TH2F *h = new TH2F("h"," ",10,-0.704000,12.256000,50,0.279857,250000.000000);

   ifstream rholev( "ni60_take3_ann-cecilie_2/rholev.cnt"), rhopaw( "ni60_take3_ann-cecilie_2/rhopaw.cnt"), fermi( "ni60_take3_ann-cecilie_2/fermigas.cnt");
   float levels[46],rho[46],rhoerr[46],energy[418],energyerr[418],fermigas[418];
   float Bn[1]={11.711000};
   float Bnerr[1]={0.001};
   float rho_Bn[1]={25000.000000};
   float rho_Bnerr[1]={3000.000000};
   int i = 0;
   float a0 =  -0.7040;
   float a1 =   0.2400;
   float x,y,z;
   while(fermi){
   	fermi >> x;
   	fermigas[i]=x;
   	energy[i]=a0+(a1*i);
   	energyerr[i]=0.0;
      i++;
   }
   i=0;
   while(rhopaw){
   	rhopaw >> y;
   	if(i<45){
   		rho[i]=y;
   	}
   	else{rhoerr[i-45]=y;}
   	i++;
   }
  	i=0;
	while(rholev){
		rholev >> z;
		levels[i]=z;
		i++;
  }
   TGraphErrors *rhoexp = new TGraphErrors(45,energy,rho,energyerr,rhoerr);
   TGraphErrors *rhoBn = new TGraphErrors(1,Bn,rho_Bn,Bnerr,rho_Bnerr);
   TGraph *fermicalc = new TGraph(417,energy,fermigas);
   TGraph *level = new TGraph(45,energy,levels);
   c1->SetLogy();
   c1->SetLeftMargin(0.14);
   h->GetXaxis()->CenterTitle();
   h->GetXaxis()->SetTitle("Excitation energy E (MeV)");
   h->GetYaxis()->CenterTitle();
   h->GetYaxis()->SetTitleOffset(1.4);
   h->GetYaxis()->SetTitle("Level density #rho (E) (MeV^{-1})");
   h->Draw();
 
   
   ld1->SetLineColor(15);
   ld1->SetLineWidth(2);
   ld1->Draw("L SAME");
   ld2->SetLineColor(2);
   ld2->SetLineWidth(2);
   ld2->Draw("L SAME");
   ld3->SetLineColor(3);
   ld3->SetLineWidth(2);
   ld3->Draw("L SAME");
   ld4->SetLineColor(4);
   ld4->SetLineWidth(2);
   ld4->Draw("L SAME");
   ld5->SetLineColor(7);
   ld5->SetLineWidth(2);
   ld5->Draw("L SAME");
   ld6->SetLineColor(6);
   ld6->SetLineWidth(2);
   ld6->Draw("L SAME");

   
   rhoexp->SetMarkerStyle(21);   
   rhoexp->SetMarkerSize(0.8);
   rhoexp->Draw("P");
   fermicalc->SetLineStyle(2);
   fermicalc->DrawGraph(42,&fermicalc->GetX()[40],&fermicalc->GetY()[40],"L");
   level->SetLineStyle(1);
   level->Draw("L");
   rhoBn->SetMarkerStyle(25);
   rhoBn->SetMarkerSize(0.8);
   rhoBn->Draw("P");
   TLegend *leg = new TLegend(0.15,0.70,0.6,0.85);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->AddEntry(rhoexp," Oslo data ","P");
   leg->AddEntry(level," Known levels ","L");
   leg->AddEntry(fermicalc," CT or FG model ","L");	
   leg->AddEntry(rhoBn," #rho from neutron res. data ","P");
      
   leg->AddEntry(ld1," TALYS ldmodel 1");
   leg->AddEntry(ld2," TALYS ldmodel 2");
   leg->AddEntry(ld3," TALYS ldmodel 3");
   leg->AddEntry(ld4," TALYS ldmodel 4");
   leg->AddEntry(ld5," TALYS ldmodel 5");
   leg->AddEntry(ld6," TALYS ldmodel 6");
   
   leg->Draw();
   TLatex t;
   t.SetTextSize(0.05);
   t.DrawLatex(    9.789,1.545e+05,"^{61}Cu");
   TArrow *arrow1 = new TArrow(0.736000,26.796258,0.736000,7.731791,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(2.896000,246.119575,2.896000,71.015335,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(4.096000,493.661802,4.096000,142.441162,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(6.976000,8087.952252,6.976000,2333.697510,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("counting_Ni60.png");
//   c1->Print("counting.eps");
//   c1->Print("counting.ps");
}
