{
	TCanvas * c {new TCanvas("c","c")};
    TGraphErrors* gsf1 = new TGraphErrors("./f1_exp_029_061_PG_1982NE06.dat", "%lg %lg %lg %lg");
    TGraphErrors* gsf2 = new TGraphErrors("./f1_exp_029_061_PG_1982NI05.dat", "%lg %lg %lg %lg");
    gsf1->SetMarkerStyle(21);
    gsf1->SetMarkerSize(0.8);
    gsf1->SetMarkerColor(2);
    gsf1->Draw("AP");
	c->SetLogy();

    gsf2->SetMarkerStyle(22);
    gsf2->SetMarkerSize(0.8);
    gsf2->SetMarkerColor(4);
    gsf2->Draw("P SAME");


    auto* legend = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend->SetTextFont(132);
    legend->AddEntry(gsf1, "Nemashkalo (1982)", "ep");
    legend->AddEntry(gsf2, "Nilson (1982)", "ep");
    legend->Draw();
}