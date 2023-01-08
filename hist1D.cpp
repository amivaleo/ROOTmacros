/*
 * Draw a simple hist from input files. 
 * Input files must contain at maximum 3 columns:	x y dy or x y ey
 * Empty lines and lines starting with # will ne ignored.
 * Errors will be considered as absolute errors.
 * If there is more than 1 input file, all hists will be superimposed.
 */

#include "MODULE.h"

void hist1D (vector<string> input, vector<string> outputFormat = {"root"}, string format = "xydy") {
	Double_t xf, yf, dyf(0);
	vector<Double_t> x, y, dy;
	string line, output, outputFile;
	Int_t point = 0;
	Int_t colorID = 2;
	
	TCanvas * c = new TCanvas("c1", "canvas", canvasWidth, canvasHeight);
	c->SetWindowSize(canvasWidth + (canvasWidth - c->GetWw()), canvasHeight + (canvasHeight - c->GetWh()));
	makeLegend();
	TLegend * legend = new TLegend(legendCoordinates.x1, legendCoordinates.y1, legendCoordinates.x2, legendCoordinates.y2);
	legend->SetTextAlign(12);

	TH1D * hist;
	gStyle->SetOptStat(false);
	if (xLog) gPad->SetLogx();
	if (yLog) gPad->SetLogy();
	
	output = "integral";
	std::ofstream fileOutput (output, ios::app);
	fileOutput << std::setw(34) << std::setfill(' ') << std::left << "# Input file" << "Integral" << std::endl;
	
	for (unsigned int i = 0; i < input.size(); i++) {
		std::ifstream fileInput (input[i].c_str());
		if (!fileInput.good()) {
			std::cerr << "ERROR :: file " << input[i] << " not found" << std::endl;
			exit(1);
		}
		
		while (getline(fileInput, line)) {
			if (line[0] == '#' || line[0] == '\0') continue;
			
			stringstream(line) >> xf >> yf >> dyf;
			
			x.push_back(xf);
			y.push_back(yf);
			dy.push_back(dyf);
		}
		
		x.insert(x.begin(), 0);
		
		hist = new TH1D(input[i].c_str(), "", y.size(), (Double_t * )&x[0]);
		
		for (unsigned int l = 0; l < y.size(); l++) {
			hist->SetBinContent(l + 1, y[l]);
			if (format == "xyey")
				hist->SetBinError(l + 1, y[1]*dy[l]);
			else
				hist->SetBinError(l + 1, dy[l]);
		}
		
		if (legendItem.size() == input.size()) {
			legend->AddEntry(hist, legendItem[i].c_str(), "lep");
		} else {
			legend->AddEntry(hist, input[i].c_str(), "lep");
		}
		
		hist->SetTitle(tTitle.c_str());
		colorID += colorID == 10 ? 1 : 0;
		hist->SetLineColor(colorID);
		hist->SetLineWidth(2);
		if (xMin != xMax) hist->GetXaxis()->SetRangeUser(xMin, xMax);
		if (yMin != yMax) hist->GetYaxis()->SetRangeUser(yMin, yMax);
		hist->Draw("HIST E0 same");
		
		
		fileOutput << std::setw(34) << std::setfill(' ') << std::left << input[i] << hist->Integral() << std::endl;
		colorID++;
		
		x.clear();
		y.clear();
		dy.clear();
	}
	
	if (input.size() > 1)
		legend->Draw();
	
	for (unsigned int i = 0; i < outputFormat.size(); i++) {
		outputFile = input[0] + "." + outputFormat[i];
		c->SaveAs(outputFile.c_str());
	}
	return;
}
