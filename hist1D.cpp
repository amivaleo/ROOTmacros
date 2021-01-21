/*
 * Draw a simple hist from input files. 
 * Input files must contain at maximum 3 columns:	x y dy
 * Available formats:
 *  x y			-> "xy"
 *  x y dy		-> "xydy"
 * Empty lines and lines starting with # will ne ignored.
 * Errors will be considered as absolute errors.
 * If there is more than 1 input file, all hists will be superimposed.
 */

void hist1D (vector<string> input, string format = "xydy") {
	const string blue("\033[1;34m");	// info
	const string green("\033[1;32m");	// request
	const string red("\033[1;31m");		// errors
	const string yellow("\033[1;33m");	// warnings

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
			cerr << red << "ERROR :: file " << input[i] << " not found" << endl;
			exit(1);
		}
		
		while (getline(fileInput, line)) {
			if (line[0] == '#' || line[0] == '\0') continue;
			
			if (format == "xydy")
				stringstream(line) >> xf >> yf >> dyf;
			else
				stringstream(line) >> xf >> yf;
			
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
		hist->Draw("HIST E0 same");
		
		x.clear();
		y.clear();
		dy.clear();
	}
	
	if (input.size() > 1)
		legend->Draw();
	
	input[0] += ".root";
	
	hist->SaveAs(input[0].c_str());
	return;
}
