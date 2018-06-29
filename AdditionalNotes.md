-----------------------------------------------------------
# MonoHiggsToGG analysis
-----------------------------------------------------------

# Setup CMSSW

The master branch of the code is compatable for CMSSW_8_0_26_patch1
Most parts should compile in CMSSW_9_2_8 as well, 
however specific instructions will likely need to be modified to 
match new releases.

# Setup flashgg

Follow all instructions from: https://github.com/cms-analysis/flashgg
Do all steps! Including compiling.

# Clone this repo

```
cd ${CMSSW_BASE}/src
git clone git@github.com:mez34/MonoHiggsToGG.git
```

# Make some modifications

In order to selection looser electrons and muons than those available in the standard flashgg package, 
need to add some lines of code to flashgg/Taggers/interface/LeptonSelection.hh and flashgg/Taggers/src/LeptonSelection.cc.
Need to define: selectLooseMuons and selectLooseElectrons and selectMediumElectrons in order for the MonoHiggs analyzer to compile. 
To define these, need to add to flashgg some changes.
After done, should be able to compile the analyzer.

Lines to add to LeptonSelection.hh: 
https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/addfiles/LeptonSelection.h#L69-L83

Lines to add to LeptonSelection.cc:
https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/addfiles/LeptonSelection.cc#L59-L111
https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/addfiles/LeptonSelection.cc#L532-L747 

These are the selection that correspond to the 2016 recommendations by EGamma.
If you want to have a different selection, you will need to modify the selection that is applied.  

# Run the Analysis

## Step 1) Get samples 

Run samples through flashgg to make microAODs.
Follow flashgg instructions.

## Step 2) Produce inputs for analysis 

### a) Setup catalogue
In the flashgg directory, once MicroAOD files are produced, 
run these scripts to create the json file (catalogue) and  compute the weights:

- `fggManageSamples.py -C CAMPAIGN -V VERSION import`
- `fggManageSamples.py -C CAMPAIGN review`
- `fggManageSamples.py -C CAMPAIGN check`

Important notes:
The "import" stage searches through DAS for datasets matching the form: *CAMPAIGN-VERSION*
and makes a long list of them.
The "review" stage allows you to select which datasets you actually want to keep.
The "check" stage makes sure that there are no errors or duplicate datasets.
These instructions are just a condensed version of the ones from flashgg:
https://github.com/cms-analysis/flashgg/blob/master/MetaData/README.md
In general, the most up-to-date instructions will be there.

### a) Extract data JSON

In the flashgg directory, extract the processed data json (used as an input in the analyzer) 
and used to compute the PU weight file: 
- `fggManageSamples.py -C CAMPAIGN getlumi <full dataset name>` 

This json corresponds to the processed (through flashgg) dataset. 
We are interested in the the AND of this json and the golden json. 
The convolution json is applyed in the analyzer to make sure we are restricted to the right portion of the data.
This is specified in the diPhoAna.py (and moriond17diPhoAnaBATCH.py) in `JSONfile`.
To get this convolution of these jsons use brilcalc:
- `compareJSON.py --and processed.json golden.json >> processedANDgolden.json`
 
### b) Get PU Weights File

Note, these instructions are for 2016. 
In general, things like minBiasXsec, inputLumiJSON, and MC pileup histogram
will have to be changed based on new results.
I've compiled some useful notes in AdditionalNotes_PUwgt.md. 

To get pileup in data, use the processedANDgolden.json produced in the previous step.
Then run (from Brilcalc):

```
pileupCalc.py -i processedANDgolden.json --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69000 --maxPileupBin 70 --numPileupBins 70  MyDataPileupHistogram.root
```

To get the PU weights file, next run the macro `pileupWeights.C` (found in macro dir.)
which uses MyDataPileupHistogram.root (output of pileupCalc.py) and compares it with a MC pileup histogram. 
This produces the `pileupWeights.root` file which is an input to the analyzer.

## Step 3) Make list and weight files 

Copy json to the scripts/list\* directory
- Copy directly from flashgg catalogue or list as produced in Step 1.
- If json file is not separated by name can extract smaller json files (from the monohiggs script dir) with:
`./runExtractJSONS.sh` which calls: `python extractJSONS.py -i input.json -o samplename -d outputdir` 

To create the .list and .weight files in list\* directory

Write the proper name of the catalogue in the extract\*.py scripts and write the name of the samples in:
- `python extractWeights.py`
- `python extractFiles.py`

OR run `./runExtractFilesAndWeights.sh` does the same thing (takes input list of .json files and outputs weight and files in same list dir.)

## Step 4) Run in local the diphoton analyzer (from python directory):
- Write by hand one microAOD file that can be taken from the json file
- Fix by hand xsec and sumDataSet that can be found from the corresponding json file
- For data, specify the `JSONfile` as mentioned earlier
- Specify the bool isMC = true or false
- Optional: put in PU reweighting file (`puWFileName`) 
- called by `cmsRun diPhoAna.py`
  
### Analyzer side notes

Anything beyond the 2016 analysis will need to update certain parts of the analyzer:

- Reweighting of the R9 and sieie, if needed, values will need to be updated (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/plugins/DiPhoAnalyzer_Moriond17.cc#L526-L530)
- MET filters (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/plugins/DiPhoAnalyzer_Moriond17.cc#L785-L805) will need to be updated based on MET POG recommendations.
- Photon ID (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/plugins/DiPhoAnalyzer_Moriond17.cc#L3528-L3558) needs to be updated following EGamma POG recommendations.
- Gain correction (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/plugins/DiPhoAnalyzer_Moriond17.cc#L4105-L4117) if still needed will be updated following EGamma POG recommendations.

 
## Step 5) Run in batch the diphoton analyzer (from script directory):
To make it works one needs:
- a list of files in script/list directory with the name of the samples 
- a list of weights in script/list directoy with the name of the samples
- the name of the list directory needs to be written in submitBatchDiPho.py
- specify the bool isMC in diPhoAnaBATCH.py
- the value of the xsec
- add optional PU reweighting & input PU reweighting file
- the output directory either in eos or in lxpus (this has to be fixed in the submitBatchDiPho.py script by hand)

Example on how to run: 
``` 
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-100to200 0 7 pippo 1534. 1 
```
NB. The name GJets_HT-100to200 has to match the one of the .list and the .weight files.

Can also run: `./submitAll_DiPhioton.py` which has the names of current samples & their xsecs
      
## Step 6) Manage the output trees before making plots 
From the macro directory:

- Merge the output files with `./mergeTrees.sh` specifying in here the output directory
- Add the weights to the trees with addWeightsToTree.cc run by `./weighTrees.sh LUMI` which weights for the provided `LUMI` (in pb^-1)
- Merge the species with `./mergeSpecies.sh`

NB. The structure of how to use these scripts can be seen in `doAll.sh`

If you add additional variables to the ntuples in the analyzer, you need to modify addWeightsToTree.cc to include these variables.

## Step 7) Produce plots 

A more streamlined plot maker is in analysis/work/
can `make` from there.
The main.cc is: https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/work/src/main.cc
The configuration is setup in: https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/work/src/Config.cc
Note a lot of the options are the same as those in the old plot making scripts.

## Step 7) Produce plots (old)

The analysis is done in CMSSW_8_0_26_patch1
- `make` (to compile) 
- `./main` (to run)
Can use `make clean` to clean.

Choose which selection to apply (whichSelection).
The ones that are currently in place:
-  if (whichSelection == 0) selName = "OrigSel";
-  if (whichSelection == 1) selName = "OptSel1";  
-  if (whichSelection == 2) selName = "OptSel2"; 
-  if (whichSelection == 3) selName = "OptSel3"; 
-  if (whichSelection == 4) selName = "OptSel4"; 

The values of the cuts on the pTs are defined in Plotter.cpp. The MET cuts are also defined in the Plotter & in ABCD.cpp.
If you want to add additional selection options, you will need to add the respective cuts to both of these.

Specify the input directory for the samples (inDir) & your output directory (outDir).
Also specify the total luminosity (lumi) in fb^-1 and the type of plots (type) wanted - i.e. png/pdf.  

In main.cpp set the following bools. 
These are the ones you will have to work with most frequently: 
- (doPlots) 	  : calls Plotter.cpp 	  --- makes the histos for each sample individually
- (doComb)  	  : calls Combiner.cpp 	  --- overlays and stacks samples in plots
- (doABCD)	  : calls ABCDMethod.cpp  --- does the ABCD C&C analysis
- (doMETCorr)	  : calls METCorr2016.cpp --- MET phi correction is calculated to t1pfmet 
- (doQCDrescale)  : rescale GJet for QCD  --- rescale GJet by QCD integral to estimate the QCD contribution
- (doBlind)	  : blinds data in Plots  --- blinds the data mass & met distributions

Bools that will likely not be needed anymore and are defaulted to false:
- (doFakeData)    : calls Plotter.cpp for fake data -- requires having a sample called FakeData.root in the samples inDir.
- (sortMC)        : before doing Combiner.cpp, this sorts the MC for plotting by smallest to biggest.
- (makePURWfiles) : calls ReweightPU.cpp  --- makes PURW files for samples)
- (doReweightPU)  : opens PURW files      --- does PURW instead of weighting=1

When you first run the plotting tools for a specific data and MC dataset, you need to run with doMETCorr = true. 
This produces the root files (in the same input dir. as the data) that has the MET Phi corrections for both Data and MC.
Then when you set this bool to false, it will gather the needed corrections from the saved root file. 

The basic flow of these tools: 
- Each sample is run individually through the Plotter.cpp which applies the appropriate selection & corrections and outputs a root file: outDir/SampleName/plots_SampleName.root and some TH2D plots in the same directory. The default is to not draw the png/pdf versions of these plots (but these lines can be uncommented in the Plotter if wanted). 
- The Combiner.cpp takes the output root file from the Plotter and does for each plot that is specified in fTH1DNames an overlay of the shapes for each MC sample and a Data/MC stack plot. Both linear & log plots are produced. Some additional special plots are produced (efficiencies, certain special shape plots etc.). Also the MET efficiencies for various corrections (taken using the cut MET > 80) are calculated and collected in outDir/ResultsSystematicsForLatex.tex . To be fixed -- change the MET > 80 to a parameter specified like in the Plotter/ABCD. 
- Optionally: ABCD.cpp does the ABCD cut-and-count analysis and produces a datacard for each signal sample, cards found in outDir/ABCD/DataCard_SIGNAME_metCUT.txt. In the same directory, 3 tables in Latex form are found (ResultsTableForLatex.tex) which has inclusive numbers, numbers in signal region, and the results of the ABCD analysis. Unfortunately, calling ABCD crashes the first time it is run -- rerunning without any changes will work.

The style for the plots is set with Style.cpp -- shouldn't need to touch this unless the formatting is off.

## Step 8) Change ntuple format for the fit
Go to analysis/fits. More specific instructions in the README there.

In short: 
Convert ntuples from FLASHgg format to format for fits by using fitterFormatting.cc 
Called by: `./formatNtupleForFitting.sh`

Choose which selection to apply by specifying the values of category: 0 to 4 are currently allowed.
Additional categories can be applied by adding the selection in fitterFormatting.cc.

## Step 9) Run the fit

Directions: https://github.com/lsoffi/MonoHggFits
More modern scripts: https://github.com/mez34/MonoHiggsToGG/tree/master/analysis/fits/DiphotonFitTools

