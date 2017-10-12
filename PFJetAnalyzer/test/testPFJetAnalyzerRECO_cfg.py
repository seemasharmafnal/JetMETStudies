import FWCore.ParameterSet.Config as cms

process = cms.Process("pfJetanalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(#'file:step3_RecoSim.root'
    #'/store/relval/CMSSW_9_2_9/RelValQCD_FlatPt_15_3000HS_13UP17/MINIAODSIM/92X_upgrade2017_realistic_Candidate_forECALStudies_HS1M_PF17-v1/00000/041633CD-C29D-E711-9553-0CC47A4D769E.root'
    # 'file:ECALZS_NoPU_MINIAODSIM.root'
    'file:ECALZS_NoPU_RECO.root'
    )
)

process.TFileService = cms.Service("TFileService",
       fileName = cms.string("myHistFile.root")
       )

process.ana = cms.EDAnalyzer('PFJetAnalyzerRECO',
  PFCandidates = cms.InputTag("particleFlow"),
  PFJets       = cms.InputTag("ak4PFJetsCHS")
)


process.p = cms.Path(process.ana)
