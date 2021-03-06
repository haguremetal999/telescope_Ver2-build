/// \file RunAction.hh
/// \brief Definition of the RunAction class

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// H1D histograms are created in BeginOfRunAction() for the following 
/// physics quantities:
/// The histograms and ntuple are saved in the output file in a format
/// accoring to a selected technology in Analysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.
///

class RunAction : public G4UserRunAction
{
  private:
  G4int Nbuff;
  //  G4int Maxhits;
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
  int GetBufsize() const {return Nbuff;};
  void SetBufsize(G4int nb) { Nbuff=nb; G4cout << "RunAction Nbuff=" << Nbuff << G4endl;}
};

#endif
