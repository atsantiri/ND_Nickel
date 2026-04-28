{
	TGraphErrors *gsf1 = new TGraphErrors("../../data_CRP_database/f1_exp_029_063_PG_1980ER09.dat","%lg %lg %lg %lg");
	TGraphErrors *gsf2 = new TGraphErrors("../../data_CRP_database/f1_exp_029_063_PG_1982NE06.dat","%lg %lg %lg %lg");
	TGraphErrors *gsf3 = new TGraphErrors("../../data_CRP_database/fe1_exp_029_063_photoneut_1964Fu1.dat","%lg %lg %lg %lg");
	TGraphErrors *gsf4 = new TGraphErrors("../../data_CRP_database/fe1_exp_029_063_photoneut_1968Su1_L0013002.dat","%lg %lg %lg %lg");
    gsf1->SetMarkerStyle(21);
	gsf1->SetMarkerSize(0.8);
	gsf1->SetMarkerColor(2);
	gsf1->Draw("P SAME");
	
	gsf2->SetMarkerStyle(22);
	gsf2->SetMarkerSize(0.8);
	gsf2->SetMarkerColor(4);
	gsf2->Draw("P SAME");

    gsf3->SetMarkerStyle(23);
	gsf3->SetMarkerSize(0.8);
	gsf3->SetMarkerColor(6);
	gsf3->Draw("P SAME");

    gsf4->SetMarkerStyle(20);
	gsf4->SetMarkerSize(0.8);
	gsf4->SetMarkerColor(15);
	gsf4->Draw("P SAME");

    auto* legend = new TLegend(0.6,0.8,0.9,0.9);
	legend->SetTextFont(132);
	legend->AddEntry(gsf1,"Erlandsson (1980) p,g","ep");
	legend->AddEntry(gsf2,"Nemashkalo (1982) p.g","ep");
	legend->AddEntry(gsf3,"Fultz (1964) photoneut","ep");
	legend->AddEntry(gsf4,"Sund (1968) photoneut","ep");
	legend->Draw();

}