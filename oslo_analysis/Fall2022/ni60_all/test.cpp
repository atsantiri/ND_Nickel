{
	TGraphErrors *gsf1 = new TGraphErrors("../data_CRP_database/f1_exp_029_061_PG_1982NE06.dat","%lg %lg %lg");
	TGraphErrors *gsf2 = new TGraphErrors("../data_CRP_database/f1_exp_029_061_PG_1982NI05.dat","%lg %lg %lg");
	gsf1->SetMarkerStyle(21);
	gsf1->SetMarkerSize(0.8);
	gsf1->SetMarkerColor(2);
	gsf1->Draw("P");
	
	gsf2->SetMarkerStyle(21);
	gsf2->SetMarkerSize(0.8);
	gsf2->SetMarkerColor(3);
	gsf2->Draw("P SAME");

}
