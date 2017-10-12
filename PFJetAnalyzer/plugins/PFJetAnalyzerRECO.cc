// -*- C++ -*-
//
// Package:    PFCalibration/PFJetAnalyzer
// Class:      PFJetAnalyzer
// 
/**\class PFJetAnalyzer PFJetAnalyzer.cc PFCalibration/PFJetAnalyzer/plugins/PFJetAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Tue, 10 Oct 2017 13:19:27 GMT
//
//

#include "JetMETStudies/PFJetAnalyzer/plugins/PFJetAnalyzerRECO.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"

//
// constructors and destructor
//
PFJetAnalyzerRECO::PFJetAnalyzerRECO(const edm::ParameterSet& iConfig) {

  ifPFCand_ = false;

  //now do what ever initialization is needed
  usesResource("TFileService");
  
  inputTagPFCandidates_ = iConfig.getParameter<edm::InputTag>("PFCandidates");
  tokenPFCandidates_    = consumes<reco::PFCandidateCollection>(inputTagPFCandidates_);

  inputTagPFJets_ = iConfig.getParameter<edm::InputTag>("PFJets");
  tokenPFJets_    = consumes<std::vector<reco::PFJet> >(inputTagPFJets_);

}


PFJetAnalyzerRECO::~PFJetAnalyzerRECO()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

// ------------ method called for each event  ------------
void PFJetAnalyzerRECO::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  // get PFCandidates and analyze them
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByToken(tokenPFCandidates_, pfCandidates);
  if(ifPFCand_){
    if(pfCandidates.isValid())
      analysePFCandidates(pfCandidates);
    else 
      std::cout << "reco::PFCandidateCollection does not exist" << std::endl;
  }

  // get miniAOD PFJets 
  edm::Handle<std::vector<reco::PFJet> > pfJets;
  //iEvent.getByLabel(inputTagPFJets_,pfJets);
  iEvent.getByToken(tokenPFJets_, pfJets);
  if(!pfJets.isValid()) std::cout<<"collections doesn't exist"<< std::endl;
  std::cout << "pfJets->size() " << pfJets->size() << std::endl;
  
  for(JI itrjet = pfJets->begin(); itrjet!=pfJets->end(); itrjet++){
    std::cout << itrjet->pt() << std::endl;
    h_pfJet_pT->Fill(itrjet->pt());
  }
  
}

void PFJetAnalyzerRECO::analysePFCandidates(edm::Handle<reco::PFCandidateCollection> pfCandidates){

  for( CI ci  = pfCandidates->begin(); ci!=pfCandidates->end(); ++ci) {

    // find pf particle type
    const reco::PFCandidate& pfc = *ci;
    //std::cout << "PFCandidate particleId = " << pfc.particleId() << std::endl;

    // Find the corresponding PF block elements
    const reco::PFCandidate::ElementsInBlocks& theElements = pfc.elementsInBlocks();
    if( theElements.empty() ) continue;

    // PFCandidate->PFBlockRef->elements
    const reco::PFBlockRef blockRef = theElements[0].first;
    reco::PFBlock::LinkData linkData =  blockRef->linkData();
    const edm::OwnVector<reco::PFBlockElement>& elements = blockRef->elements();

    // for each element, check type
    for(unsigned iEle=0; iEle<elements.size(); iEle++) {
   
      // Find the tracks in the block
      reco::PFBlockElement::Type type = elements[iEle].type();
      //std::cout << "element type " << type << std::endl;
      
      if(type == reco::PFBlockElement::TRACK) {
	const reco::PFBlockElementTrack& trk =
	  dynamic_cast<const reco::PFBlockElementTrack &>( elements[iEle] );
	double p   = trk.trackRef()->p();  
	double pt  = trk.trackRef()->pt(); 
	double eta = trk.trackRef()->eta();
	double phi = trk.trackRef()->phi();
	std::cout<<"element track :p, pt, eta, phi " << p << " " << pt << " " << eta << " " << phi << std::endl;
      } else if(type == reco::PFBlockElement::ECAL) {
	const reco::PFBlockElementCluster& eecal =
	  dynamic_cast<const reco::PFBlockElementCluster &>( elements[iEle] );
	double e   = eecal.clusterRef()->energy();  
	double eta = eecal.clusterRef()->eta();
	double phi = eecal.clusterRef()->phi();
	std::cout<<"element ecal :e, eta, phi " << e << " " << eta << " " << phi << std::endl;
      } else if(type == reco::PFBlockElement::HCAL) {
	const reco::PFBlockElementCluster& ehcal =
	  dynamic_cast<const reco::PFBlockElementCluster &>( elements[iEle] );
	double e   = ehcal.clusterRef()->energy();  
	double eta = ehcal.clusterRef()->eta();
	double phi = ehcal.clusterRef()->phi();
	std::cout<<"element hcal :e, eta, phi " << e << " " << eta << " " << phi << std::endl;
	// get rechit reference for this cluster
	const std::vector< reco::PFRecHitFraction > erh = ehcal.clusterRef()->recHitFractions();
	for(unsigned int ieh=0;ieh<erh.size();ieh++) {
	  std::cout << "rechit frac " <<  erh[ieh].fraction() << std::endl;
	  // the following didn't work
	  //std::cout << " rechitRef " << erh[ieh].recHitRef()->energy() << std::endl;
	}

      }
   
    
    } // loop over block elements

  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
PFJetAnalyzerRECO::beginJob()
{
  bookHitograms();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PFJetAnalyzerRECO::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PFJetAnalyzerRECO::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void PFJetAnalyzerRECO::bookHitograms() {

  h_pfJet_pT = fs->make<TH1F>("h_pfJet_pT", "h_pfJet_pT", 100, 0, 100.0);  
}

//define this as a plug-in
DEFINE_FWK_MODULE(PFJetAnalyzerRECO);
