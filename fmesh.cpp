/*
 * Draw a simple hist from input files. 
 * Input files must contain at maximum 3 columns:	x y dy
 * Available formats:
 *  x y			-> "xy"
 *  x y dy		-> "xydy"
 * If there is more than 1 input file, all hists will be superimposed.
 */

bool is_number(const std::string&);

void fmesh (string input, string option = "1", double min = 1E-99) {
	unsigned int row(0), jumpedRows(15);
	string line, word1, word2, word3;
	double value;
	double axisD;
	char axis;
	double max;
	
	vector<double> xb;
	vector<double> yb;
	vector<double> zb;

	ifstream fileInput (input.c_str());

	while (getline(fileInput, line)) {
		if (line[0] == '#' || line[0] == '\0' || line[0] == ' ') continue;

		stringstream(line) >> word1 >> word2;

		if (word1 == "X" && word2 == "direction:") {
			istringstream iss(line);
			while(iss >> word1) {
				if (is_number(word1))
					xb.push_back(stod(word1));
			}
		} else if (word1 == "Y" && word2 == "direction:") {
			istringstream iss(line);
			while(iss >> word1) {
				if (is_number(word1))
					yb.push_back(stod(word1));
			}
		} else if (word1 == "Z" && word2 == "direction:") {
			istringstream iss(line);
			while(iss >> word1) {
				if (is_number(word1))
					zb.push_back(stod(word1));
			}
		} else
			continue;
	}
	


	const unsigned int xs(xb.size() - 1), ys(yb.size() - 1), zs(zb.size() - 1);
	
	TH3D * h3d = new TH3D("h3d", "3dhist", xb.size() - 1, (Double_t *) &xb[0], yb.size() - 1, (Double_t *) &yb[0], zb.size() - 1, (Double_t *) &zb[0]);
	
	ifstream fileInput2 (input.c_str());	
	while (getline(fileInput2, line)) {
		stringstream(line) >> word1 >> word2 >> word2 >> word2 >> word3;
		if (!is_number(word1)) continue;
		cout << word1 << "1t" << word3 << endl;
		h3d->SetBinContent((row) / zs / ys % xs + 1, (row) / zs % ys + 1, (row) % zs + 1, stod(word2));
		h3d->SetBinError((row) / zs / ys % xs + 1, (row) / zs % ys + 1, (row) % zs + 1, stod(word3));
		row++;
	}
	
	if (option == "1") {
		cout << "Which option [""xy"", ""xz"", etc]? ";
		cin >> option;
	}

	word1 = option;
	option.replace(0, 1, option, 1, 1);
	option.replace(1, 1, word1, 0, 1);

	if (option.find("x") > 2)
		axis = 'x';
	else if (option.find("y") > 2)
		axis = 'y';
	else if (option.find("z") > 2)
		axis = 'z';

	switch (axis) {
		case 'x':
			if (min == 1E-99) {
				cout << " X bin boundaries: ";
				for (unsigned int i = 0; i <= h3d->GetXaxis()->GetNbins(); i++) {
					cout << h3d->GetXaxis()->GetBinUpEdge(i) << "\t";
				}
				cout << endl;
				do {
					cout << "Which LOW boundary? ";
					cin >> min;
				} while (h3d->GetXaxis()->FindFixBin(min) < 1);
			}
			h3d->GetXaxis()->SetRangeUser(h3d->GetXaxis()->GetBinLowEdge(h3d->GetXaxis()->FindFixBin(min)), h3d->GetXaxis()->GetBinUpEdge(h3d->GetXaxis()->FindFixBin(min)));
		break;
		
		case 'y':
			if (min == 1E-99) {
				cout << " Y bin boundaries: ";
				for (unsigned int i = 0; i <= h3d->GetYaxis()->GetNbins(); i++) {
					cout << h3d->GetYaxis()->GetBinUpEdge(i) << "\t";
				}
				cout << endl;
				do {
					cout << "Which LOW boundary? ";
					cin >> min;
				} while (h3d->GetYaxis()->FindFixBin(min) < 1);
			}
			h3d->GetYaxis()->SetRangeUser(h3d->GetYaxis()->GetBinLowEdge(h3d->GetYaxis()->FindFixBin(min)), h3d->GetYaxis()->GetBinUpEdge(h3d->GetYaxis()->FindFixBin(min)));
		break;
		
		case 'z':
			if (min == 1E-99) {
				cout << " Z bin boundaries: ";
				for (unsigned int i = 0; i <= h3d->GetZaxis()->GetNbins(); i++) {
					cout << h3d->GetZaxis()->GetBinUpEdge(i) << "\t";
				}
				cout << endl;
				do {
					cout << "Which LOW boundary? ";
					cin >> min;
				} while (h3d->GetZaxis()->FindFixBin(min) < 1);
			}
			
			h3d->GetZaxis()->SetRangeUser(h3d->GetZaxis()->GetBinLowEdge(h3d->GetZaxis()->FindFixBin(min)), h3d->GetZaxis()->GetBinUpEdge(h3d->GetZaxis()->FindFixBin(min)));
		break;
	}
	
	TH2D * h2d = (TH2D *)h3d->Project3D(option.c_str());
	word1.insert(1, ";");
	option = (string)h2d->GetTitle() + " & " + axis + ": [" + h3d->GetZaxis()->GetBinLowEdge(h3d->GetZaxis()->FindFixBin(min)) + ", " + h3d->GetZaxis()->GetBinUpEdge(h3d->GetZaxis()->FindFixBin(min)) + "];" + word1 + ";" + axis;
	
	h2d->SetTitle(option.c_str());


	TCanvas * c = new TCanvas();
	gStyle->SetOptStat(false);
	h2d->Draw("colz");
	
	input += "_PR" + word1 + "_AT" + axis + h3d->GetZaxis()->GetBinLowEdge(h3d->GetZaxis()->FindFixBin(min)) + "to" + h3d->GetZaxis()->GetBinUpEdge(h3d->GetZaxis()->FindFixBin(min)) + ".root";
	h2d->SaveAs(input.c_str());
}

bool is_number(const std::string& s) {
	char* end = nullptr;
	double val = strtod(s.c_str(), &end);
	return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}
