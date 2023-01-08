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
	double xf, dxf, yf, dyf;
	vector<double> x, y;
	string line;
	unsigned int fitFunctionType;
	unsigned int nParameters;
	vector<string> fitFunction;
	ofstream functionTypes ("/usr/share/root/macros/functionTypes", ios::app);
	ifstream functionTypesRead ("/usr/share/root/macros/functionTypes");
	
	fitFunction.push_back("<user defined fitFunction>");
	
	// short list of suggested functions
	while (getline(functionTypesRead, line)) {
		fitFunction.push_back(line);
	}
	
	TGraphErrors * graph = new TGraphErrors();
	TGraph * residue = new TGraph();
	
	TCanvas * c = new TCanvas("c1", "canvas", 1800, 600);
	c->Divide(2, 1);
	c->cd(1);
	gPad->SetTicks();
	gPad->SetGrid();
	gStyle->SetOptFit(111);
	
	// Read the input file
	ifstream fileInput (input);
	while (getline(fileInput, line)) {
		if (line[0] == '#' || line[0] == '\0' || line[0] == ' ') continue;
	
		if (format == "xdxydy")
			stringstream(line) >> xf >> dxf >> yf >> dyf;
		else if (format == "xydy") {
			stringstream(line) >> xf >> yf >> dyf;
			dxf = 0;
		} else if (format == "xdxy") {
			stringstream(line) >> xf >> dxf >> yf;
			dyf = 0;
		} else {
			stringstream(line) >> xf >> yf;
			dxf = 0;
			dyf = 0;
		}
		
		x.push_back(xf);
		y.push_back(yf);
		
		graph->SetPoint(point, xf, yf);
		graph->SetPointError(point, dxf, dyf);
		point++;
	}
	
	graph->SetTitle("; x; y");
	graph->SetMarkerStyle(8);
	graph->Draw("APE");
	c->cd(1)->Update();
	
	xMin = x[0];
	xMax = x[x.size() - 1];
	
	// Let the user choose the x range for the fit
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
	for (size_t i = 0; i < fitFunction.size(); ++i)
		cout << "	" << i << " :: " << "	" << fitFunction[i] << endl;
	
	do {
		cout << "Choose the fit function [0 - " << fitFunction.size() - 1 << "]: ";
		cin >> fitFunctionType;
	} while (fitFunctionType > fitFunction.size() - 1);
	
	// if a custom function is chosen
	if (fitFunctionType == 0) {
		cout << "Do not insert spaces!" << endl;
		cout << "No syntax validation is performed!" << endl;
		cout << "Visit https://root.cern.ch/root/html524/TMath.html for using special functions." << endl;
		cout << "Insert custom fitFunction: ";
		cin >> fitFunction[0];
		
		for (size_t i = 0; i < fitFunction.size(); i++) {
			if (i == 0)
				continue;
			if (fitFunction[0] == fitFunction[i]) {
				cout << "The inserted function is already in the set at line " << i << endl;
				rangeChoice = 'e';
				break;
			}
		}
	}
	
	if (rangeChoice != 'e') {
		do {
			cout << "Would you like to save the inserted function for next uses? [Y/N] ";
			cin >> rangeChoice;
		} while (rangeChoice != 'Y' && rangeChoice != 'N' && rangeChoice != 'y' && rangeChoice != 'n');
				
		if (rangeChoice == 'Y' || rangeChoice == 'y')
			functionTypes << fitFunction[0];
	}
	
	TF1 * fit = new TF1 ("fit", fitFunction[fitFunctionType].c_str(), xMin, xMax);
	
	nParameters = count(fitFunction[fitFunctionType].begin(), fitFunction[fitFunctionType].end(), '[');
	definePar(fit, nParameters);
	
	fit->SetLineColor(2);
	fit->SetLineWidth(3);
	
	// Draw the fit and add the function in the graph title
	graph->Fit("fit");
	graph->SetTitle(fitFunction[fitFunctionType].c_str());
	c->cd(1)->Update();
	
	// Show the residual r = fit(x) - y
	c->cd(2);
	gPad->SetTicks();
	gPad->SetGrid();
	gStyle->SetOptFit(111);
	
	point = 0;
	cout << "Residuals:" << endl;
	cout << "fitted\tdata" << endl;
	for (unsigned int i = 0; i < x.size(); i++) {
		residue->SetPoint(point, x[point], fit->Eval(x[point]) - y[point]);
		++point;
	}
	
	residue->SetTitle("Residuals; x; f(x) - y");
	residue->SetMarkerStyle(8);
	residue->Draw("APL");
	
	x.clear();
	y.clear();
	
	c->SaveAs((input + ".root").c_str());
	return;
}

void definePar(TF1 * fit, unsigned int nParameters) {
	double value;
	cout << "Parameter fist guess:" << endl;
	for (unsigned int i = 0; i < nParameters; i++) {
		cout << "	" << "[" << i << "] :: ";
		cin >> value;
		fit->SetParameter(i, value);
	}
	return;
}
