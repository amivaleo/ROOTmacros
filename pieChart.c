/*
 * Draw a pie chart from input file. The input file must contain only
 * 2 column: labels | values
 */

#include "MODULE.h"

void pieChart (string input) {
	string line, labA, labB, labC;
	size_t point = 0;
	string labelf;
	double xf;
	vector<string> label;
	vector<double> x;
	
	TCanvas * c = new TCanvas(input.c_str(), input.c_str(), 600, 500);
	c->SetFixedAspectRatio(1);
	gStyle->SetOptStat(false);
		
	const double ratio = (double)c->GetWw() / (double)c->GetWh();
		
	TPad * square = new TPad("square", "square", 0.0, 0.0, 1.0 / ratio, 1.0);
	square->Draw();
	square->cd();
	
	ifstream fileInput (input);
	while (getline(fileInput, line)) {
		if (line[0] == '\0') continue;
		
		if (line[0] == '#') {
			stringstream(line) >> labA >> tTitle >> labA >> labB >> labC;
			continue;
		}
		
		stringstream(line) >> labelf >> xf;
		
		label.push_back(labelf);
		x.push_back(xf);
		point++;
	}
	
	tTitle += " [" + labA + " " + labB + "] " + labC;
	
	TPie * pie = new TPie(fileName.c_str(), fileName.c_str(), point, (Double_t * ) &x[0]);
	pie->SetTitle(tTitle.c_str());
	pie->SetTextSizePixels(22);
	pie->SetCircle(0.5, 0.47, 0.45);
	pie->SetLabelFormat("");
	
	for (size_t i = 0; i < label.size(); i++) {
		label[i] += "	" + to_string(x[i]*100/(sumVector(x))) + "%";
		pie->SetEntryLabel(i, label[i].c_str());
		pie->SetEntryVal(i, x[i]);
		pie->SetEntryFillColor(i, i+2);	
		pie->SetEntryLineColor(i, i+2);
	}
	
	pie->Draw("nol");
	
	c->cd();
	
	TLegend * legend = pie->MakeLegend();
	legend->SetX1(0.82);
	legend->SetX2(0.98);
	if (point > 23) {
		legend->SetNColumns(2);
		point /= 2;
		legend->SetX1(0.72);
	}
	legend->SetY1(0.98 - ++point*0.036);
	legend->SetY2(0.98);
	
	line = fileName + ".pdf";
	c->SaveAs(line.c_str());
	line = fileName + ".root";
	c->SaveAs(line.c_str());
	
	label.clear();
	x.clear();
	
	return;
}
