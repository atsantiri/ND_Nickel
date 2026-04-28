{
   TGraph *ld1 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %lg %*lg %*lg %*lg %*lg %*lg");	
   TGraph *ld2 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %*lg %lg %*lg %*lg %*lg %*lg");	
   TGraph *ld3 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %*lg %*lg %lg %*lg %*lg %*lg");	
   TGraph *ld4 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %*lg %*lg %*lg %lg %*lg %*lg");	
   TGraph *ld5 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %*lg %*lg %*lg %*lg %lg %*lg");	
   TGraph *ld6 = new TGraph("../../talys/ld_allparity_all_cu59","%lg %*lg %*lg %*lg %*lg %*lg %lg");	


   gROOT->Reset();
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetFillColor(0);
   gStyle->SetPadBorderMode(0);
   m = (TH1F*)gROOT->FindObject("h");
   if (m) m->Delete();
   TCanvas *c1 = new TCanvas("c1","Normalization of level density",600,600);
   TH2F *h = new TH2F("h"," ",10,-0.884000,13.196000,50,0.181217,500000.000000);
   ifstream rholev("rholev.cnt"), rhopaw("rhopaw.cnt"), fermi("fermigas.cnt");
   float levels[32],rho[32],rhoerr[32],energy[314],energyerr[314],fermigas[314];
   float Bn[1]={12.766000};
   float Bnerr[1]={0.001};
   float rho_Bn[1]={18000.000000};
   float rho_Bnerr[1]={200.000000};
   int i = 0;
   float a0 =  -0.8840;
   float a1 =   0.3200;
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
   	if(i<31){
   		rho[i]=y;
   	}
   	else{rhoerr[i-31]=y;}
   	i++;
   }
  	i=0;
	while(rholev){
		rholev >> z;
		levels[i]=z;
		i++;
  }
   TGraphErrors *rhoexp = new TGraphErrors(31,energy,rho,energyerr,rhoerr);
   TGraphErrors *rhoBn = new TGraphErrors(1,Bn,rho_Bn,Bnerr,rho_Bnerr);
   TGraph *fermicalc = new TGraph(313,energy,fermigas);
   TGraph *level = new TGraph(31,energy,levels);
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
   
   rhoexp->SetMarkerStyle(21);   rhoexp->SetMarkerSize(0.8);
   rhoexp->Draw("P");
   fermicalc->SetLineStyle(2);
   fermicalc->DrawGraph(30,&fermicalc->GetX()[15],&fermicalc->GetY()[15],"L");
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
   t.DrawLatex(   10.557,2.500e+05,"^{59}Cu");
   TArrow *arrow1 = new TArrow(1.996000,64.989018,1.996000,17.496170,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(3.596000,406.606634,3.596000,109.465553,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(3.916000,679.792648,3.916000,183.011963,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(5.516000,3090.500264,5.516000,832.016235,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("counting_Ni58.png");
//   c1->Print("counting.eps");
//   c1->Print("counting.ps");
}
