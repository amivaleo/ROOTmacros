/*
 * Draw a simple graph from input files. 
 * Input files must contain at maximum 3 columns:	x dx y dy
 * Available formats:
 *  x y			-> "xy"
 *  x y dy		-> "xydy"
 *  x dx y     -> "xdxy"
 *  x dx y dy  -> "xdxydy"
 * Empty lines and lines starting with # will ne ignored.
 * Errors will be considered as absolute errors.
 * If there is more than 1 input file, all graphs will be superimposed.
 */
 
#include "MODULE.h"

void graph1D (vector<string> input, vector<string> outputFormat = {"root"}, string format = "xy") {
	const string blue("\033[1;34m");	// info
	const string green("\033[1;32m");	// request
	const string red("\033[1;31m");		// errors
	const string yellow("\033[1;33m");	// warnings
	
	Int_t colorID = 2;

	string line, outputFile;
	unsigned int point = 0;
	Double_t x, y, dx(0), dy(0);
	
	TCanvas * c = new TCanvas();
	TLegend * legend = new TLegend();
	legend->SetTextAlign(12);
	TGraphErrors * graph;
	
	for (unsigned int i = 0; i < input.size(); i++) {
		graph = new TGraphErrors();
		graph->SetMarkerSize(1);
		graph->SetMarkerStyle(20);
		
		if (legendItem.size() == input.size()) {
			legend->AddEntry(graph, legendItem[i].c_str(), "lep");
		} else {
			legend->AddEntry(graph, input[i].c_str(), "lep");
		}
		
		ifstream fileInput (input[i].c_str());
		if (!fileInput.good()) {
			cerr << red << "ERROR :: file " << input[i] << " not found" << endl;
			exit(1);
		}
		
		while (getline(fileInput, line)) {
			if (line[0]=='#' || line[0]=='\0') continue;
			
			if (format == "xdxydy")
				stringstream(line) >> x >> dx >> y >> dy;
			else if (format == "xydy")
				stringstream(line) >> x >> y >> dy;
			else if (format == "xdxy")
				stringstream(line) >> x >> dx >> y;
			else
				stringstream(line) >> x >> y;
			
			graph->SetPoint(point, x, y);
			graph->SetPointError(point, dx, dy);
			
			point++;
		}
		
		point = 0;
		
		graph->SetTitle(tTitle.c_str());
		graph->SetTitle(tTitle.c_str());
		colorID += colorID == 10 ? 1 : 0;
		graph->SetLineColor(colorID);
		graph->SetLineWidth(2);
		graph->SetMarkerColor(graph->GetLineColor());
		graph->SetTitle((input[i] + ";x;y").c_str());
		if (xMin != xMax) graph->GetXaxis()->SetRangeUser(xMin, xMax);
		if (yMin != yMax) graph->GetYaxis()->SetRangeUser(yMin, yMax);
		
		if (i > 0)
			graph->Draw("PLE");
		else
			graph->Draw("APLE");
		
		colorID++;
	}
	
	if (input.size() > 1)
		legend->Draw();
	
	for (unsigned int i = 0; i < outputFormat.size(); i++) {
		outputFile = input[0] + "." + outputFormat[i];
		c->SaveAs(outputFile.c_str());
	}
	return;
}
