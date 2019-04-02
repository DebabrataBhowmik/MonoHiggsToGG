from CRABClient.UserUtilities import config
config = config()

config.General.requestName   = 'DiPhotonJetsBox_newID_rightPU'
config.General.transferLogs = True
config.JobType.pluginName  = 'Analysis'

config.JobType.inputFiles  = ['input_files/transformation_Moriond17_AfterPreApr_v1.root', 'input_files/pu_Weights_DiPhotonJetsBox.root', 'input_files/Run2017_17Nov2017_v1_ele_smearings.dat', 'input_files/Run2017_17Nov2017_v1_ele_scales.dat']

# Name of the CMSSW configuration file                                                                        
 
config.JobType.psetName    = 'diPhoAna.py'

#config.Data.inputDataset = '/SingleElectron/Run2016F-HcalCalIsoTrkFilter-PromptReco-v1/ALCARECO'                                                                      
#config.Data.inputDataset = '/DoublePion_E-50/PhaseIFall16DR-FlatPU20to50RECO_81X_upgrade2017_realistic_v26-v1/GEN-SIM-RECO'            
                               
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-2_7_7-2_7_7-v0-Run2017C-17Nov2017-v1-07c3a951e9177a6fb1470ee3c7814a6f/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-2_7_5-2_7_5-v0-Run2017C-PromptReco-v1-55dd8a3831767cdcb092d16987c90615/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-3_0_1-3_0_1-v0-Run2017B-17Nov2017-v1-a18f46d96a188391b5acca7d030a860e/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-3_1_0-3_1_0-v0-Run2017B-31Mar2018-v1-aa3a4880c1f02b5f7421114b3c220c90/USER'
config.Data.inputDataset = '/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/sethzenz-RunIIFall17-3_1_1-3_1_1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2-dfb4aebb0a34a36b04b8bd8eb2ddfe46/USER'

config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
#config.Data.inputDBS = 'phys03'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'

config.Data.splitting = 'FileBased'                                                                                                                                    
#config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1

#config.Data.outLFNDirBase = '/store/user/%s/MHgg/SignalDEG_2017Data' % (getUsernameFromSiteDB())
config.Data.outLFNDirBase = '/store/user/dbhowmik/MHgg/Bkg/newID_rightPUweight/BkgDiPhotonJetsBox_2017MC'
config.Data.publication = False

#config.Data.ignoreLocality = True
#config.Site.whitelist = ['T2_CH_CERN']
# Where the output files will be transmitted to                                                                                                                        
 
config.Site.storageSite = 'T3_US_FNALLPC'
