// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include <TFile.h>
#include <TH1F.h>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PFJetAnalyzerRECO : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
 public:
  explicit PFJetAnalyzerRECO(const edm::ParameterSet&);
  ~PFJetAnalyzerRECO();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  

 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  bool ifPFCand_;
  void analysePFCandidates(edm::Handle<reco::PFCandidateCollection>);
  
  typedef reco::PFCandidateCollection::const_iterator CI;
  edm::InputTag   inputTagPFCandidates_;
  edm::EDGetTokenT<reco::PFCandidateCollection> tokenPFCandidates_;

  typedef std::vector<reco::PFJet>::const_iterator JI;
  edm::InputTag   inputTagPFJets_;
  edm::EDGetTokenT<std::vector<reco::PFJet> > tokenPFJets_;

  void bookHitograms();
  edm::Service<TFileService> fs;
  TH1F *h_pfJet_pT, *h_pfJet_eta, *h_pfJet_phi;
  
};
