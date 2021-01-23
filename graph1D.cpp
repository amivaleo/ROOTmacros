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

void graph1D (vector<string> input, string format = "xy") {
	const string blue("\033[1;34m");	// info
	const string green("\033[1;32m");	// request
	const string red("\033[1;31m");		// errors
	const string yellow("\033[1;33m");	// warnings

	string line;
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
		
		legend->AddEntry(graph, input[i].c_str(), "lep");
		
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
		
		graph->SetLineColor(i + 2);
		graph->SetMarkerColor(graph->GetLineColor());
		graph->SetTitle((input[i] + ";x;y").c_str());
		
		if (i > 0)
			graph->Draw("PLE");
		else
			graph->Draw("APLE");
	}
	
	if (input.size() > 1)
		legend->Draw();
	
	input[0] += ".root";

	graph->SaveAs(input[0].c_str());
	return;
}
