#include "MODULE.h"

void polar_graph(string IN)
{
  // +++ VARIABLES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  string line;
  int point = 0;
  double val, ray;
    
  Double_t amin=0;
  Double_t amax=TMath::Pi()*2;

  
  vector<double> angle;
  vector<double> rho;
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // +++ READ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ifstream fileInput (IN.c_str());
  while ( getline(fileInput, line) )
  {
    if (line[0]=='#'||line[0]=='\0') continue;
    stringstream(line) >> val >> ray;
/*    if (cosine)*/
/*      val = acos(val);*/
    
    angle.push_back(val);
    rho.push_back(ray);
    point++;
  }
  
  rho = rinomalizeVector(rho);
  
  Double_t a[angle.size()];
  Double_t r[rho.size()];
  
  for (size_t i = 0; i < angle.size(); i++)
  {
    a[i] = angle[i];
    r[i] = rho[i];
  }
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // +++ CANVAS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TCanvas *c = new TCanvas("c%d", "Polar Graph", 800, 800);
  c->SetLeftMargin(0.07);
  c->SetTopMargin(0.02);
  c->SetRightMargin(0.02);
  c->SetBottomMargin(0.07);
  if (grid) c->SetGrid();
  if (xLog) {gPad->SetLogx();}
  if (yLog) {gPad->SetLogy();}
  gStyle->SetOptStat(kFALSE);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // +++ DRAW +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TGraphPolar * polar_graph = new TGraphPolar(angle.size(),a,r);
  polar_graph->SetTitle(title.c_str());
  polar_graph->SetLineWidth(1);
  polar_graph->SetLineColor(30);
  polar_graph->SetFillStyle(1001);
  polar_graph->SetFillColor(30);

  polar_graph->Draw("AFL");
  c->Update();
  polar_graph->GetPolargram()->SetPolarOffset(0.008);
  polar_graph->GetPolargram()->SetPolarLabelSize(0.03);
  polar_graph->GetPolargram()->SetRadialOffset(0.008);
  polar_graph->GetPolargram()->SetRadialLabelSize(0.03);
  
  yMax = getMaxVectorValue(rho);
  if (yMin != yMax) polar_graph->GetPolargram()->SetRangeRadial(yMin, yMax);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   
  // +++ SAVE TO FILE +++++++++++++++++++++++++++++++++++++++++++++++++++++++
  int file_counter = 0;
  string output = fileName.c_str() + to_string(file_counter) + ".eps";
  while (file_counter != -1)
  {
    if (fileExist(output.c_str()))
    {
      file_counter++;
      output = fileName.c_str() + to_string(file_counter) + ".eps";
    }
    else
    {
      file_counter = -1;
    }
  }
  c->Print(output.c_str(), "eps");
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}
