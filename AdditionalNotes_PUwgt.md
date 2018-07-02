Convert FLASHgg JSON to Normal JSON (makes one JSON per one input):
* cd ${CMSSW_BASE}/src/flashgg/MetaData/data/ 
* fggManageSamples.py -C [DIRECTORY] getlumi /DoubleEG/sethzenz-RunIIFall17-* 

Combine Processed JSONS (add all DoubleEG jsons, can only add 2 at a time):
* compareJSON.py --or [JSON1] [JSON2] >> output_name.json  

Find Golden JSON: /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/

Extract Processed & Golden JSON (used for masking in analyzer):
* compareJSON.py --and [Processed JSON] [Golden JSON] >> processed_and_golden.json

Lumi Calculation:
* brilcalc lumi --normtag=/afs/cern.ch/user/l/lumipro/public/normtag_file/moriond16_normtag.json -u /pb -i processed_and_golden.json

Get PU from Data (use processed_and_golden.json):
* pileupCalc.py -i processed_and_golden.json --inputLumiJSON 
/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt
--calcMode true --minBiasXsec 69000 --maxPileupBin 52 --numPileupBins 52  MyDataPileupHistogram.root

Get PU from MC:
* The distribution is produced centrally and it is documented for each MC campaign on the PdMV twiki:
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis#Monte_Carlo
* For the 2016 production it was:https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/SimGeneral/MixingModule/python/mix_2016_25ns_Moriond17MC_PoissonOOTPU_cfi.py
* The values put in a histo are "probValue"

Get PU reweight root file (used to weight in analyzer): 
* cd ${CMSSW_BASE}/src/MonoHiggsToGG/analysis/macro/
* pileupWeights.root  ** run, replacing MC and DATA pileup files **
 
-----------------------------------------------------------------------------------------------------
USEFUL LINKS:
* PdmV: https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2017Analysis
* Bril:     https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html
* Lumi:   https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM
* Pileup: https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData
-----------------------------------------------------------------------------------------------------
