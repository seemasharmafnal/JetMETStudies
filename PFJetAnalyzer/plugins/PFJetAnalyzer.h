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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PFJetAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
 public:
  explicit PFJetAnalyzer(const edm::ParameterSet&);
  ~PFJetAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  

 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  void analysePFCandidates(edm::Handle<reco::PFCandidateCollection>);
  
  typedef reco::PFCandidateCollection::const_iterator CI;
  edm::InputTag   inputTagPFCandidates_;
  edm::EDGetTokenT<reco::PFCandidateCollection> tokenPFCandidates_;

  //typedef edm::View<pat::Jet>::const_iterator pfJetItr;
  //typedef std::vector<pat::Jet>::const_iterator pfJetItr;
  edm::InputTag   inputTagPFJets_;
  edm::EDGetTokenT<std::vector<pat::Jet> > tokenPFJets_;
  
};
