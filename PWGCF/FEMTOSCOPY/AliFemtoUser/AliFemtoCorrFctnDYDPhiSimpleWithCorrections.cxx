////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// AliFemtoCorrFctnDYDPhiSimple - A correlation function that analyzes        //
// two particle correlations with respect to the azimuthal angle (phi)        //
// and rapidity (y) difference                                                //
//                                                                            //
//                                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "AliFemtoCorrFctnDYDPhiSimpleWithCorrections.h"
#include "AliFemtoCorrFctnDYDPhi.h"
#include "AliFemtoModelHiddenInfo.h"
//#include "AliFemtoHisto.hh"
#include <cstdio>
#include <TMath.h>

#ifdef __ROOT__
ClassImp(AliFemtoCorrFctnDYDPhiSimpleWithCorrections)
#endif

#define PIH 1.57079632679489656
#define PIT 6.28318530717958623

//____________________________
AliFemtoCorrFctnDYDPhiSimpleWithCorrections::AliFemtoCorrFctnDYDPhiSimpleWithCorrections(const char* title, const int& aPhiBins=20, const int& aYBins=20, const double& mass1=0, const double& mass2=0):
 AliFemtoCorrFctn(),
  fDPhiDYNumerator(0),
  fDPhiDYDenominator(0),
  fPhi(0),
  fY(0),
  fphiL(0),
  fphiT(0),
  fMass1(0),
  fMass2(0)
{
  fphiL = (-(int)(aPhiBins/4)+0.5)*2.*TMath::Pi()/aPhiBins;
  fphiT = 2*TMath::Pi()+(-(int)(aPhiBins/4)+0.5)*2.*TMath::Pi()/aPhiBins;
  fMass1 = mass1;
  fMass2 = mass2;


  // set up numerator
  char tTitNumD[101] = "NumDPhiDY";
  strncat(tTitNumD,title, 100);
  fDPhiDYNumerator = new TH2D(tTitNumD,title,aPhiBins,fphiL,fphiT,aYBins,-2.0,2.0);
  // set up denominator
  char tTitDenD[101] = "DenDPhiDY";
  strncat(tTitDenD,title, 100);
  fDPhiDYDenominator = new TH2D(tTitDenD,title,aPhiBins,fphiL,fphiT,aYBins,-2.0,2.0);


  char tTitPhi[101] = "Phi";
  strncat(tTitPhi,title, 100);
  fPhi = new TH1D(tTitPhi,title,90,-TMath::Pi(),TMath::Pi());

  char tTitY[101] = "Y";
  strncat(tTitY,title, 100);
  fY = new TH1D(tTitY,title,90,-1.2,1.2);


  // to enable error bar calculation...
  fDPhiDYNumerator->Sumw2();
  fDPhiDYDenominator->Sumw2();
  fPhi->Sumw2();
  fY->Sumw2();
}

//____________________________
AliFemtoCorrFctnDYDPhiSimpleWithCorrections::AliFemtoCorrFctnDYDPhiSimpleWithCorrections(const AliFemtoCorrFctnDYDPhiSimpleWithCorrections& aCorrFctn) :
  AliFemtoCorrFctn(),
  fDPhiDYNumerator(0),
  fDPhiDYDenominator(0),
  fPhi(0),
  fY(0),
  fphiL(0),
  fphiT(0),
  fMass1(0),
  fMass2(0)
{
  

  // copy constructor
  if (aCorrFctn.fDPhiDYNumerator)
    fDPhiDYNumerator = new TH2D(*aCorrFctn.fDPhiDYNumerator);
  else
    fDPhiDYNumerator = 0;

  if (aCorrFctn.fDPhiDYDenominator)
    fDPhiDYDenominator = new TH2D(*aCorrFctn.fDPhiDYDenominator);
  else
    fDPhiDYDenominator = 0;

 if (aCorrFctn.fPhi)
    fPhi = new TH1D(*aCorrFctn.fPhi);
  else
    fPhi = 0;

 if (aCorrFctn.fY)
    fY = new TH1D(*aCorrFctn.fY);
  else
    fY = 0;


  fphiL = aCorrFctn.fphiL;
  fphiT = aCorrFctn.fphiT;
  fMass1 = aCorrFctn.fMass1;
  fMass2 = aCorrFctn.fMass2;

}
//____________________________
AliFemtoCorrFctnDYDPhiSimpleWithCorrections::~AliFemtoCorrFctnDYDPhiSimpleWithCorrections()
{
  // destructor

  delete fDPhiDYNumerator;
  delete fDPhiDYDenominator;

  delete fPhi;
  delete fY;


}
//_________________________
AliFemtoCorrFctnDYDPhiSimpleWithCorrections& AliFemtoCorrFctnDYDPhiSimpleWithCorrections::operator=(const AliFemtoCorrFctnDYDPhiSimpleWithCorrections& aCorrFctn)
{
    // assignment operator
  if (this == &aCorrFctn)
    return *this;

  if (aCorrFctn.fDPhiDYNumerator)
    fDPhiDYNumerator = new TH2D(*aCorrFctn.fDPhiDYNumerator);
  else
    fDPhiDYNumerator = 0;
  if (aCorrFctn.fDPhiDYDenominator)
    fDPhiDYDenominator = new TH2D(*aCorrFctn.fDPhiDYDenominator);
  else
    fDPhiDYDenominator = 0;

  if (aCorrFctn.fPhi)
    fPhi = new TH1D(*aCorrFctn.fPhi);
  else
    fPhi = 0;

  if (aCorrFctn.fY)
    fY = new TH1D(*aCorrFctn.fY);
  else
    fY = 0;


  fphiL = aCorrFctn.fphiL;
  fphiT = aCorrFctn.fphiT;
  fMass1 = aCorrFctn.fMass1;
  fMass2 = aCorrFctn.fMass2;

  return *this;

}
//_________________________
void AliFemtoCorrFctnDYDPhiSimpleWithCorrections::Finish()
{
  // here is where we should normalize, fit, etc...
  // we should NOT Draw() the histos (as I had done it below),
  // since we want to insulate ourselves from root at this level
  // of the code.  Do it instead at root command line with browser.
  //  mShareNumerator->Draw();
  //mShareDenominator->Draw();
  //mRatio->Draw();

}

//____________________________
AliFemtoString AliFemtoCorrFctnDYDPhiSimpleWithCorrections::Report()
{
  // create report
  string stemp = "TPC Ncls Correlation Function Report:\n";
  char ctemp[100];
  snprintf(ctemp , 100, "Number of entries in numerator:\t%E\n",fDPhiDYNumerator->GetEntries());
  stemp += ctemp;
  snprintf(ctemp , 100, "Number of entries in denominator:\t%E\n",fDPhiDYDenominator->GetEntries());
  stemp += ctemp;
  //  stemp += mCoulombWeight->Report();
  AliFemtoString returnThis = stemp;
  return returnThis;
}
//____________________________
void AliFemtoCorrFctnDYDPhiSimpleWithCorrections::AddRealPair (AliFemtoPair* pair)
{
  // add real (effect) pair
  if (fPairCut && !fPairCut->Pass(pair)) {
    return;
  }
 double px1 = pair->Track1()->FourMomentum().px();
  double py1 = pair->Track1()->FourMomentum().py();
  double pz1 = pair->Track1()->FourMomentum().pz();

  double px2 = pair->Track2()->FourMomentum().px();
  double py2 = pair->Track2()->FourMomentum().py();
  double pz2 = pair->Track2()->FourMomentum().pz();

  double p1 = TMath::Sqrt(px1*px1+py1*py1+pz1*pz1);
  double p2 = TMath::Sqrt(px2*px2+py2*py2+pz2*pz2);

  double E1 = TMath::Sqrt(p1*p1+fMass1*fMass1);
  double E2 = TMath::Sqrt(p2*p2+fMass2*fMass2);

  double y1 = 0.5*TMath::Log((E1+pz1)/(E1-pz1));
  double y2 = 0.5*TMath::Log((E2+pz2)/(E2-pz2));

  double phi1 = pair->Track1()->FourMomentum().Phi();
  double phi2 = pair->Track2()->FourMomentum().Phi();


  double dphi = phi1 - phi2;
  while (dphi<fphiL) dphi+=PIT;
  while (dphi>fphiT) dphi-=PIT;

 double dy = y1 - y2;
 

  float weight = 1;

  if(pair->Track1()->Track()){
    if(part1==kPion) weight = pair->Track1()->Track()->CorrectionPion();
    else if(part1==kKaon) weight = pair->Track1()->Track()->CorrectionKaon();
    else if(part1==kProton) weight = pair->Track1()->Track()->CorrectionProton();
    else if(part1==kPionMinus) weight = pair->Track1()->Track()->CorrectionPionMinus();
    else if(part1==kKaonMinus) weight = pair->Track1()->Track()->CorrectionKaonMinus();
    else if(part1==kProtonMinus) weight = pair->Track1()->Track()->CorrectionProtonMinus();
    else if(part1==kAll) weight = pair->Track1()->Track()->CorrectionAll();
  }
  if(pair->Track1()->V0()){
    if(part1==kLambda) weight = pair->Track1()->V0()->CorrectionLambda();
    if(part1==kLambdaMinus) weight = pair->Track1()->V0()->CorrectionLambdaMinus();
  }

  if(pair->Track2()->Track()){
    if(part2==kPion) weight *= pair->Track2()->Track()->CorrectionPion();
    else if(part2==kKaon) weight *= pair->Track2()->Track()->CorrectionKaon();
    else if(part2==kProton) weight *= pair->Track2()->Track()->CorrectionProton();
    else if(part2==kPionMinus) weight *= pair->Track2()->Track()->CorrectionPionMinus();
    else if(part2==kKaonMinus) weight *= pair->Track2()->Track()->CorrectionKaonMinus();
    else if(part2==kProtonMinus) weight *= pair->Track2()->Track()->CorrectionProtonMinus();
    else if(part2==kAll) weight *= pair->Track2()->Track()->CorrectionAll();
  }
  if(pair->Track2()->V0()){
    if(part2==kLambda)
      weight *= pair->Track2()->V0()->CorrectionLambda();
    if(part2==kLambdaMinus)
      weight *= pair->Track2()->V0()->CorrectionLambdaMinus();
  }

  fDPhiDYNumerator->Fill(dphi, dy, weight);



 

}

//____________________________
void AliFemtoCorrFctnDYDPhiSimpleWithCorrections::AddMixedPair( AliFemtoPair* pair)
{
  // add mixed (background) pair
  if (fPairCut && !fPairCut->Pass(pair)) {
    return;
  }

  double px1 = pair->Track1()->FourMomentum().px();
  double py1 = pair->Track1()->FourMomentum().py();
  double pz1 = pair->Track1()->FourMomentum().pz();

  double px2 = pair->Track2()->FourMomentum().px();
  double py2 = pair->Track2()->FourMomentum().py();
  double pz2 = pair->Track2()->FourMomentum().pz();

  double p1 = TMath::Sqrt(px1*px1+py1*py1+pz1*pz1);
  double p2 = TMath::Sqrt(px2*px2+py2*py2+pz2*pz2);

  double E1 = TMath::Sqrt(p1*p1+fMass1*fMass1);
  double E2 = TMath::Sqrt(p2*p2+fMass2*fMass2);

  double y1 = 0.5*TMath::Log((E1+pz1)/(E1-pz1));
  double y2 = 0.5*TMath::Log((E2+pz2)/(E2-pz2));

  double phi1 = pair->Track1()->FourMomentum().Phi();
  double phi2 = pair->Track2()->FourMomentum().Phi();


  double dphi = phi1 - phi2;
  while (dphi<fphiL) dphi+=PIT;
  while (dphi>fphiT) dphi-=PIT;

  double dy = y1 - y2;

  float weight = 1;

  if(pair->Track1()->Track())
  {
    if(part1==kPion) weight = pair->Track1()->Track()->CorrectionPion();
    else if(part1==kKaon) weight = pair->Track1()->Track()->CorrectionKaon();
    else if(part1==kProton) weight = pair->Track1()->Track()->CorrectionProton();
    else if(part1==kPionMinus) weight = pair->Track1()->Track()->CorrectionPionMinus();
    else if(part1==kKaonMinus) weight = pair->Track1()->Track()->CorrectionKaonMinus();
    else if(part1==kProtonMinus) weight = pair->Track1()->Track()->CorrectionProtonMinus();
    else if(part1==kAll) weight = pair->Track1()->Track()->CorrectionAll();
   }
  if(pair->Track1()->V0()){
    if(part1==kLambda) weight = pair->Track1()->V0()->CorrectionLambda();
    else if(part1==kLambdaMinus) weight = pair->Track1()->V0()->CorrectionLambdaMinus();
  }

  if(pair->Track2()->Track()){
    if(part2==kPion) weight *= pair->Track2()->Track()->CorrectionPion();
    else if(part2==kKaon) weight *= pair->Track2()->Track()->CorrectionKaon();
    else if(part2==kProton) weight *= pair->Track2()->Track()->CorrectionProton();
    else if(part2==kPionMinus) weight *= pair->Track2()->Track()->CorrectionPionMinus();
    else if(part2==kKaonMinus) weight *= pair->Track2()->Track()->CorrectionKaonMinus();
    else if(part2==kProtonMinus) weight *= pair->Track2()->Track()->CorrectionProtonMinus();
    else if(part2==kAll) weight *= pair->Track2()->Track()->CorrectionAll();
   }
  if(pair->Track2()->V0()){
    if(part2==kLambda) weight *= pair->Track2()->V0()->CorrectionLambda();
    else if(part2==kLambdaMinus) weight *= pair->Track2()->V0()->CorrectionLambdaMinus();
  }
  if(pair->Track2()->Xi()){
    if(part2==kXiMinus) weight *= pair->Track2()->Xi()->CorrectionXiMinus();
    else if(part2==kXiPlus) weight *= pair->Track2()->Xi()->CorrectionXiPlus();
  }
                 
  fDPhiDYDenominator->Fill(dphi, dy, weight);

}


void AliFemtoCorrFctnDYDPhiSimpleWithCorrections::WriteHistos()
{
  // Write out result histograms
  fDPhiDYNumerator->Write();
  fDPhiDYDenominator->Write();
  
}

TList* AliFemtoCorrFctnDYDPhiSimpleWithCorrections::GetOutputList()
{
  // Prepare the list of objects to be written to the output
  TList *tOutputList = new TList();

  tOutputList->Add(fDPhiDYNumerator);
  tOutputList->Add(fDPhiDYDenominator);

  tOutputList->Add(fPhi);
  tOutputList->Add(fY);

  return tOutputList;

}
