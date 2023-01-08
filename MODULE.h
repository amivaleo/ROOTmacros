#ifndef module_h
#define module_h

/*----------------------------------------------------------------------------*/

const string tab("\t");

const string blue("\033[1;34m");	// info
const string green("\033[1;32m");	// request
const string pink("\033[1;35m");	// test
const string red("\033[1;31m");		// errors
const string yellow("\033[1;33m");	// warnings

const string warning("\033[1;33m⚠ \033[0m\e[0m");

const string reset("\033[0m\e[0m");

const double leth = 10/TMath::Log(10);	// multiplier for lethargy units

bool fileExist(const string fileName) {
	ifstream infile(fileName);
	return infile.good();
}

const vector<string> axes = {"f", "d", "u", "s", "m", "c", "e", "t", "i", "j", "k"};
vector<int> axesBin = { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

size_t getAxisIndex(const string element) {
	auto it = find(axes.begin(), axes.end(), element);
	if (it != axes.end()) return distance(axes.begin(), it);
	else return -1;
}

vector<string> saveFormat = {"pdf", "root"};
/*----------------------------------------------------------------------------*/

bool edit = 1;	// show editor
bool tbar = 1;	// show toolbar
bool verb = 1;	// verbose?
bool tick = 1;	// show axes ticks?
bool grid = 1;	// show grid?
bool xLab(1), yLab(1), zLab(1);	// show values on x, y, z axes
bool xLog(0), yLog(1), zLog(0);	// x, y, z axes log-scale

int xAxis(-1), yAxis(-1), zAxis(-1);

double xMul(1), yMul(1), zMul(1);
double xMin(-99999), yMin(-99999), zMin(-99999);
double xMax(-99999), yMax(-99999), zMax(-99999);

struct box {
	double x1 = 0.88;
	double y1 = 0.12;
	double x2 = 0.98;
	double y2 = 0.88;
} legendCoordinates;

double pMin(-99999), pMax(-99999);

vector<double> contour;

std::vector<string> legendItem = {
	"Poly: 55 cm",
	"Poly: 50 cm",
	"Poly: 45 cm",
	"Poly: 40 cm",
	"Poly: 35 cm",
	"Poly: 30 cm",
	"Poly: 25 cm",
	"Poly: 20 cm",
	"Poly: 15 cm"
	};


string fileName = "";
string imgName = "";

string tTatle = " ;Channel [#];#Phi uncollided / #Phi total";
string tTitle(" ;E [MeV];Neutron flux per source particle [n/cm^{2}];E [MeV];Neutron flux per source particle [n/cm^{2}]");
//	"Neutron flux on surface 5.35 x 5.50 cm2 at BW exit";
//	"Integrated neutron flux";
//	"Neutron dose rate [#muSv/h]";
//	"f21 n at x 1500 cm on surf 5x5cm2, angle 0.2 - 0.1 deg";
//	"Neutron flux per unit lethargy [n/cm^{2}/sec] @ Bunker Wall exit";
//	"Neutron flux per unit lethargy [n/cm^{2}/sec] @ Bunker Wall exit (z: 13.7 cm)";
//	"Neutron flux per unit lethargy [n/cm^{2}/sec] @ 25m from TCS";
//	"Conversion functions from flux to dose rate";
//	"2GeV neutron transmittivity in natural Cu";
//	"#xi as a function of x";
//	"Conversion function from flux to dose rate";
//	"Neutron flux @ (x, y, z) = (550.0, 0.0, 13.7) (units in cm) (monolith exit)";

string xTitle =
//	"x [cm]";
//	" z";
//	"#xi";
//	"[deg]";	
	"E [MeV]";

string yTitle = 
//	"y [cm]";
//	"f(#xi)";
//	"Neutron flux [arbitrary units]";
//	"Neutron flux on a surface in lethargy units [cm^{-2} sec^{-1}]";
//	"Neutron current [n/sec]";
//	"n/sec";
//	"#muSv/h";
	"Neutron flux in lethargy units [cm^{-2} sec^{-1}]";

string zTitle =
//	"z [cm]";
	"";
//	"[#mu Sv / h]";

int canvasWidth(1600), canvasHeight(1000);
TImage * img;

TCanvas * generateCanvas(int cW = canvasWidth, int cH = canvasHeight) {
	gStyle->SetOptStat(0);
	if (fileExist(imgName.c_str())) {
		img = TImage::Open(imgName.c_str());
		cW = img->GetWidth() * 2;
		cH = img->GetHeight() * 2;
	}
//	TCanvas * c = new TCanvas((to_string(rand())).c_str(), (to_string(rand())).c_str(), cW, cH);
	TCanvas * c = new TCanvas(fileName.c_str(), fileName.c_str(), cW, cH);
	// 			L R D U
	c->SetMargin(0.1, 0.028, 0.1, 0.03);
	
	if (xMul != 1) cout << warning << "x axis is multiplied by " << xMul << reset << endl;
	if (yMul != 1) cout << warning << "y axis is multiplied by " << yMul << reset << endl;
	if (zMul != 1) cout << warning << "z axis is multiplied by " << zMul << reset << endl;
	
	if (xLog) {
		gPad->SetLogx();
		if (verb) cout << warning << "x axis in log-scale" << reset << endl;
	}
	if (yLog) {
		gPad->SetLogy();
		if (verb) cout << warning << "y axis in log-scale" << reset << endl;
	}
	if (zLog) {
		gPad->SetLogz();
		if (verb) cout << warning << "z axis in log-scale" << reset << endl;
	}
	
	if (tbar) c->ToggleEventStatus();
	if (edit) c->ToggleEditor();
	if (grid) c->SetGrid();
	if (tick) gPad->SetTicks();
	return c;
}

TLegend * generateLegend(unsigned int size = 0) {
	TLegend * legend = new TLegend(legendCoordinates.x1, legendCoordinates.y1, legendCoordinates.x2, legendCoordinates.y2);
	legend->SetNColumns(1);
	legend->SetLineColor(1);
	legend->SetTextAlign(12);
	return legend;
}

void makeLegend() {
	double legendWidth = 0.5;
	for (std::string s : legendItem) {
		legendWidth = legendWidth < s.length()*0.012 ? legendWidth : s.length()*0.012;
	}
	legendCoordinates.x2 = 0.98;
	cout << legendWidth << endl;
	legendCoordinates.x1 = legendCoordinates.x2 - legendWidth;
	
	double legendHeight = 0.5;
	legendCoordinates.y2 = legendCoordinates.y1 + legendItem.size()*0.046;
	
	return;
}

void customizeHist(TH1 * hist) {
	hist->SetTitle(tTitle.c_str());
	hist->GetXaxis()->SetTitle(xTitle.c_str());
	hist->GetYaxis()->SetTitle(yTitle.c_str());
	hist->GetZaxis()->SetTitle(zTitle.c_str());
	hist->GetZaxis()->SetTitleOffset(1.3);
	hist->SetLineWidth(2);
	gStyle->SetNumberContours(50);
	gStyle->SetPalette(kRainBow,0,0.9);
	if (xMin != xMax) hist->GetXaxis()->SetRangeUser(xMin, xMax);
	if (yMin != yMax) hist->GetYaxis()->SetRangeUser(yMin, yMax);
	if (!xLab) {
		hist->GetXaxis()->SetLabelSize(0);
		hist->GetXaxis()->SetTitleOffset(0.5);
	}
	if (!yLab) {
		hist->GetYaxis()->SetLabelSize(0);
		hist->GetYaxis()->SetTitleOffset(0.5);
	}
	if (!zLab) {
		hist->GetZaxis()->SetLabelSize(0);
		hist->GetZaxis()->SetTitleOffset(0.5);
	}
	return;
}

void customizePie(TPie * pie) {
	pie->SetTitle(tTitle.c_str());
	return;
}

void customizeGraph(TGraphErrors * graph) {
	graph->SetTitle(tTitle.c_str());
	graph->GetXaxis()->SetTitle(xTitle.c_str());
	graph->GetYaxis()->SetTitle(yTitle.c_str());
	graph->SetLineWidth(3);
	graph->SetMarkerSize(1);
	graph->SetMarkerStyle(20);
	graph->SetLineColor(1);
	if (xMin != xMax) graph->GetXaxis()->SetRangeUser(xMin, xMax);
	if (yMin != yMax) graph->GetYaxis()->SetRangeUser(yMin, yMax);
	if (!xLab) graph->GetXaxis()->SetLabelSize(0);
	if (!yLab) graph->GetYaxis()->SetLabelSize(0);
	return;
}

void customizeGraph2D(TGraph2D * graph) {
	graph->SetTitle(tTitle.c_str());
	graph->GetXaxis()->SetTitle(xTitle.c_str());
	graph->GetYaxis()->SetTitle(yTitle.c_str());
	graph->GetZaxis()->SetTitle(zTitle.c_str());
	graph->SetLineWidth(3);
	graph->SetMarkerSize(1);
	graph->SetMarkerStyle(20);
	if (xMin != xMax) graph->GetXaxis()->SetRangeUser(xMin, xMax);
	if (yMin != yMax) graph->GetYaxis()->SetRangeUser(yMin, yMax);
	if (zMin != zMax) graph->GetYaxis()->SetRangeUser(zMin, zMax);
	if (!xLab) graph->GetXaxis()->SetLabelSize(0);
	if (!yLab) graph->GetYaxis()->SetLabelSize(0);
	if (!zLab) graph->GetZaxis()->SetLabelSize(0);
	return;
}

void customizeMultiGraph(TMultiGraph * graph) {
	graph->SetTitle(tTitle.c_str());
	graph->GetXaxis()->SetTitle(xTitle.c_str());
	graph->GetYaxis()->SetTitle(yTitle.c_str());
	if (!xLab) graph->GetXaxis()->SetLabelSize(0);
	if (!yLab) graph->GetYaxis()->SetLabelSize(0);
	return;
}

void save(TCanvas * c) {
	string output;
	for (string f : saveFormat) {
		output = (string)(c->GetName()) + "." + f;
		c->SaveAs(output.c_str());
	}
}

bool isNumber(string s) {
	return (s.find_first_not_of("0123456789") == string::npos);
}

size_t countLines(const string file) {
	string line;
	size_t lineNumber = 0;
	ifstream inFile(file.c_str()); 
	while (getline(inFile, line)) {
		if (line[0]=='#'||line[0]=='\0') continue;
		lineNumber++;
	}		
	return lineNumber;
}

double getMaxVectorValue(vector<double>& vec) {
	double max;
	if (!vec.empty()) {
		max = vec.front() ;
		for(size_t i = 1; i < vec.size(); i++)	{
			if(vec[i] > max ) max = vec[i];
		}
	}
	return max;
}

vector<double> nomalizeVector(vector<double>& vec, double max = 1) {
	vector<double> rin;
	if (max == 1)
		max = getMaxVectorValue(vec);
	for (double s : vec)
		rin.push_back(s/max);
	return rin;
}

double sumVector(vector<double>& vec) {
	double sum = 0;
	for (double s : vec)
		sum += s;
	return sum;
}

//unsigned color () {
//	srand (time(NULL));
//	return rand() % 899 + 2;
//}

//	while (getline(file_input, line)) {
//		istringstream iss(line);
//		while (iss >> value) {}
//	}

//	for(std::vector<double>::iterator it = V.begin(); it != V.elementWg.end(); ++it) {

#endif
