#include "MODULE.h"

void definePar(TF1 *, size_t);

void rootFit (string input) {

	edit = 1;

	size_t fitFunctionType;
	size_t nParameters;
	vector<string> fitFunction = {
		"<user defined fitFunction>",
		"[0] + [1]*x",
		"[0] + [1]*x + [2]*x**2",
		"[0] + [1]*x + [2]*x**2 + [3]*x**3",
		"[0] + [1]*x + [2]*x**2 + [3]*x**3 + [4]*x**4",
		"[0] + [1]*exp([2]*x-[3])",
		"[0] + [1]*x**[2] * (1-x)**[3]",
		"[0] + [1]*x**[2]*exp([3]*x**[4])"
	};
	
	cout << blue << "Available default fit fitFunction" << reset << endl;
	for (size_t i = 0; i < fitFunction.size(); ++i)
		cout << blue << tab << i << " :: " << tab << fitFunction[i] << reset << endl;
	
	do {
		cout << green << "Choose the fit function [0 - " << fitFunction.size() - 1 << "]: " << reset;
		cin >> fitFunctionType;
	} while (fitFunctionType > fitFunction.size());
	
	if (fitFunctionType == 0) {
		cout << yellow << "⚠ No syntax validation is performed!" << reset << endl;
		cout << green << "Insert custom fitFunction: " << reset << endl;
		cin >> fitFunction[0];
	}
	
	nParameters = count(fitFunction[fitFunctionType].begin(), fitFunction[fitFunctionType].end(), '[');
	
	tTitle = fitFunction[fitFunctionType];
	
	
	TFile *file = new TFile(input.c_str(), "READ");
	TGraphErrors *g = (TGraphErrors *)file->Get(";1");
	TF1* fit = new TF1 ("fit", fitFunction[fitFunctionType].c_str(), xMin, xMax);
	customizeGraph(g);
	
	definePar(fit, nParameters);
	
	gStyle->SetOptFit(111);
	TCanvas *c = generateCanvas();
	
	g->Draw("APLE");
	
	if (xMin == xMax) {
		xMin = g->GetXaxis()->GetXmin();
		xMax = g->GetXaxis()->GetXmax();
	}
	
	g->Fit("fit");
	
	cout << blue << "(x, y)(min) = " << fit->GetMinimumX(xMin, xMax) << ", " << fit->GetMinimum(xMin, xMax) << endl;
	
	TPaveText *pave = new TPaveText(0.65, 0.60, 0.95, 0.65, "NDC");
	pave->SetFillColor(0);
	pave->SetBorderSize(1);
	string minimumLabel = "min(f(#xi)) = (" + to_string(fit->GetMinimumX(xMin, xMax)) + ", " + to_string(fit->GetMinimum(xMin, xMax)) + ")";
	pave->AddText(minimumLabel.c_str());
	pave->Draw();
	
//	cout << g->GetPoint(1, 1, 1) << endl;
//	cout << fit->Eval(0.002) << endl;
	
	file->Close();
	fileName = input + "fit";
	file = 0;
	save(c);
}

void definePar(TF1 * fit, size_t nParameters) {
	double value;
	cout << green << "Parameter fist guess:" << reset << endl;
	for (size_t i = 0; i < nParameters; i++) {
		cout << green << tab << "[" << i << "] :: " << reset;
		cin >> value;
		fit->SetParameter(i, value);
	}
	return;
}
