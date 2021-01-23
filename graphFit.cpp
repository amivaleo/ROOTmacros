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
 
void definePar(TF1 *, unsigned int);

void graphFit(string input, string format = "xy") {
	char rangeChoice;	
	double xMin, xMax;
	
	unsigned int point = 0;
	double xf, dxf, yf, dyf, chi2(0);
	vector<double> x, y;
	string line;
	
	unsigned int fitFunctionType;
	unsigned int nParameters;
	vector<string> fitFunction = {
		"<user defined fitFunction>",
		"[0]+[1]*x",
		"[0]+[1]*exp([2]*x**[3]-[4])",
		"[0]+[1]*x+[2]*x**2",
		"[0]+[1]*x+[2]*x**2+[3]*x**3",
		"[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4",
		"[0]+[1]*x**[2]*exp([3]*x**[4])",
		"[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**[6]*exp([7]*x**[8])",
		"[0]+[1]*x**[2]*(1-x)**[3]",
		"[0]+[1]*x**[2]+[3]*x**[4]*exp([5]*x**[6])"
	};
	
	
	TGraphErrors * graph = new TGraphErrors();
	TGraph * residue = new TGraph();
	
	TCanvas * c = new TCanvas("c1", "canvas", 1800, 600);
	c->Divide(2, 1);
	c->cd(1);
	gPad->SetTicks();
	gPad->SetGrid();
	gStyle->SetOptFit(111);
	
	ifstream fileInput (input);
	while (getline(fileInput, line)) {
		if (line[0] == '#' || line[0] == '\0') continue;
	
		if (format == "xdxydy")
			stringstream(line) >> xf >> dxf >> yf >> dyf;
		else if (format == "xydy")
			stringstream(line) >> xf >> yf >> dyf;
		else if (format == "xdxy")
			stringstream(line) >> xf >> dxf >> yf;
		else
			stringstream(line) >> xf >> yf;
		
		x.push_back(xf);
		y.push_back(yf);
		
		graph->SetPoint(point, xf, yf);
		graph->SetPointError(point, dxf, dyf);
		point++;
	}
	
	graph->SetTitle("; x; y");
	graph->Draw("APLE");
	c->cd(1)->Update();
	
	xMin = x[0];
	xMax = x[x.size() - 1];
	
	do {
		cout << "Set fitting region [Y/N]? (If N, the total range [" << xMin << "; " << xMax << "] will be used) ";
		cin >> rangeChoice;
	} while (rangeChoice != 'Y' && rangeChoice != 'N' && rangeChoice != 'y' && rangeChoice != 'n');
	
	if (rangeChoice == 'Y' || rangeChoice == 'y') {
		cout << " Set minimum x: ";
		cin >> xMin;
		cout << " Set maximum x: ";
		cin >> xMax;
	}
	
	cout << "Available default fit fitFunctions:" << endl;
	for (unsigned int i = 0; i < fitFunction.size(); ++i)
		cout << "	" << i << " :: " << "	" << fitFunction[i] << endl;
	
	do {
		cout << "Choose the fit function [0 - " << fitFunction.size() - 1 << "]: ";
		cin >> fitFunctionType;
	} while (fitFunctionType > fitFunction.size());
	
	if (fitFunctionType == 0) {
		cout << "Do not insert spaces!" << endl;
		cout << "No syntax validation is performed!" << endl;
		cout << "Insert custom fitFunction: ";
		cin >> fitFunction[0];
	}
	
	TF1 * fit = new TF1 ("fit", fitFunction[fitFunctionType].c_str(), xMin, xMax);
	
	nParameters = count(fitFunction[fitFunctionType].begin(), fitFunction[fitFunctionType].end(), '[');
	definePar(fit, nParameters);
	
	fitFunction[fitFunctionType];
	
	graph->Fit("fit");
	graph->SetTitle(fitFunction[fitFunctionType].c_str());
	c->cd(1)->Update();

	c->cd(2);
	gPad->SetTicks();
	gPad->SetGrid();
	gStyle->SetOptFit(111);
	
	point = 0;
	cout << "Residuals:" << endl;
	cout << "fitted\tdata" << endl;
	for (unsigned int i = 0; i < x.size(); i++) {
		cout << fit->Eval(x[point]) << "\t" << y[point] << endl;
		residue->SetPoint(point, x[point], fit->Eval(x[point]) - y[point]);
		++point;
	}
	
	residue->SetTitle("Residuals; x; f(x) - y");
	residue->SetMarkerStyle(21);
	residue->Draw("APL");
	
	point = 0;
	for (unsigned int i = 0; i < x.size(); ++i) {
		if (y[i] == 0) break;
		chi2 += TMath::Power(fit->Eval(x[i]) - y[i], 2) / y[i];
		++point;
	}
	chi2 /= point - 1;
	
//	cout << "chi2: " << chi2 << endl;
	
	x.clear();
	y.clear();
	
	graph->SaveAs((input + ".root").c_str());
	return;
}

void definePar(TF1 * fit, unsigned int nParameters) {
	const string green("\033[1;32m");	// request
	double value;
	cout << "Parameter fist guess:" << endl;
	for (unsigned int i = 0; i < nParameters; i++) {
		cout << "	" << "[" << i << "] :: ";
		cin >> value;
		fit->SetParameter(i, value);
	}
	return;
}
