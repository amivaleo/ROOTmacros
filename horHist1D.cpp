/*
 * Draw a simple horizontal hist from input files. 
 * Input files must contain at maximum 3 columns:	x y dy
 * Empty lines and lines starting with # will ne ignored.
 * Errors will be considered as absolute errors.
 * If there is more than 1 input file, all hists will be superimposed.
 */

#include "MODULE.h"

void horHist1D (vector<string> input, vector<string> output = {"root"}) {
	Double_t xf, yf, dyf(0);
	vector<Double_t> x, y, dy;
	string line;
	Int_t point = 0;
	Int_t color = 1;
		
	TCanvas * c = new TCanvas();
	TLegend * legend = new TLegend();
	legend->SetTextAlign(12);

	TH1D * hist;
	gStyle->SetOptStat(false);
	
	for (unsigned int i = 0; i < input.size(); i++) {
		ifstream fileInput (input[i].c_str());
		if (!fileInput.good()) {
			cerr << "ERROR :: file " << input[i] << " not found" << endl;
			exit(1);
		}
		
		while (getline(fileInput, line)) {
			if (line[0] == '#' || line[0] == '\0' || line[0] == ' ') continue;
			
			stringstream(line) >> xf >> yf >> dyf;
			
			x.push_back(xf);
			y.push_back(yf);
			dy.push_back(dyf);
		}
		
		x.insert(x.begin(), 0);
		
		hist = new TH1D(input[i].c_str(), "", y.size(), (Double_t * )&x[0]);
		
		for (unsigned int l = 0; l < y.size(); l++) {
			hist->SetBinContent(l + 1, y[l]);
			hist->SetBinError(l + 1, dy[l]);
		}
		
		legend->AddEntry(hist, input[i].c_str(), "lep");
		
		hist->SetTitle((input[i] + ";x;y").c_str());
		hist->SetLineColor(i + 2);
		hist->SetFillColor(4);
		hist->SetBarWidth(0.94);
		hist->SetBarOffset(0.02);
		if (xMin != xMax) hist->GetXaxis()->SetRangeUser(xMin, xMax);
		if (yMin != yMax) hist->GetYaxis()->SetRangeUser(yMin, yMax);
		hist->Draw("hbar same");
		
		x.clear();
		y.clear();
		dy.clear();
	}
	
	if (input.size() > 1)
		legend->Draw();
	
	for (unsigned int i = 0; i < output.size(); i++) {
		output[i] = input[0] + "." + output[i];
		c->SaveAs(output[i].c_str());
	}
	return;
}
