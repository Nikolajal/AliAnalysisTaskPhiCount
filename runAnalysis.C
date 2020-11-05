// TODO LIST
// TODO: You're all set!

#include "AliAnalysisTaskPhiCount.h"
#include "AliAnalysisTaskPIDResponse.h"

// MC Correspondant: LHC14j4b
std::vector<int>    LHC10b = { 117222, 117220, 117116, 117112, 117099, 117092, 117063, 117060, 117059, 117053, 117052, 117050, 117048, 116645, 116643, 116574, 116571, 116562, 116403, 116402, 116288, 116102, 116081, 116079, 115414, 115401, 115399, 115393, 115345, 115335, 115328, 115322, 115318, 115310, 115193, 115186, 114931, 114930, 114924, 114918, 114798, 114786 };


// MC Correspondant: LHC14j4c
std::vector<int>    LHC10c = { 121040, 121039, 120829, 120825, 120824, 120823, 120822, 120821, 120758, 120750, 120741, 120671, 120617, 120616, 120505, 120503, 120244, 120079, 120076, 120073, 120072, 120069, 120067, 119862, 119859, 119856, 119853, 119849, 119846, 119845, 119844, 119842, 119841, 118561, 118560, 118558, 118556, 118518, 118506 };


// MC Correspondant: LHC14j4d
std::vector<int>    LHC10d = { 126158 , 126097, 126090, 126088, 126082, 126081, 126078, 126073, 126008, 126007, 126004, 125855, 125851, 125850, 125849, 125848, 125847, 125844, 125843, 125842, 125633, 125632, 125630, 125628, 125296, 125134, 125101, 125100, 125097, 125085, 125083, 125023, 122375, 122374 };

// MC Correspondant: LHC14j4e
std::vector<int>    LHC10e = { 130850, 130848, 130847, 130844, 130842, 130840, 130834, 130799, 130798, 130795, 130793, 130704, 130696, 130628, 130623, 130621, 130620, 130609, 130608, 130524, 130520, 130519, 130517, 130481, 130480, 130479, 130375, 130178, 130172, 130168, 130158, 130157, 130149, 129983, 129966, 129962, 129961, 129960, 129744, 129742, 129738, 129736, 129735, 129734, 129729, 129726, 129725, 129723, 129666, 129659, 129653, 129652, 129651, 129650, 129647, 129641, 129639, 129599, 129587, 129586, 129540, 129536, 129528, 129527, 129525, 129524, 129523, 129521, 129520, 129514, 129513, 129512, 129042, 128913, 128855, 128853, 128850, 128843, 128836, 128835, 128834, 128833, 128824, 128823, 128820, 128819, 128778, 128777, 128678, 128677, 128621, 128615, 128611, 128609, 128605, 128582, 128506, 128505, 128504, 128503, 128498, 128495, 128494, 128486 };

void runAnalysis( string fOption = "", Int_t kPeriod = -1)
{
    Bool_t MCFlag = false;
    Bool_t local = true;
    Bool_t gridTest = true;
    Bool_t KaonFlag = false;
    Bool_t PhiFlag = true;
    if ( fOption.find("Full") != -1 )
    {
        local = false;
        gridTest = false;
    }
    if ( fOption.find("Test") != -1 )
    {
        local = false;
        gridTest = true;
    }
    if ( fOption.find("MC") != -1 )
    {
        MCFlag = true;
    }
    if ( fOption.find("Kaon") != -1 )
    {
        KaonFlag = true;
        PhiFlag = false;
    }
    if ( fOption.find("AllTree") != -1 )
    {
        KaonFlag = true;
        PhiFlag = true;
    }
    if ( fOption.find("Stat") != -1 )
    {
        KaonFlag = false;
        PhiFlag = false;
    }
        
    std::vector<int> RunList;
    auto RunYear = "";
    auto RunName = "";
    
    if ( MCFlag )   cout << "[info]: MC Run ENABLED" << endl;
    else            cout << "[info]: MC Run DISABLED" << endl;
    
    if ( local )    cout << "[info]: Local analysis ENABLED" << endl;
    else
    {
        if ( gridTest ) cout << "[info]: GridTest ENABLED" << endl;
        else            cout << "[info]: GridTest DISABLED" << endl;
    }
    
    switch ( kPeriod )
    {
    case 1:
        if ( MCFlag )
        {
            RunYear = "2014";
            RunName = "LHC14j4c";
        }
        else
        {
            RunYear = "2010";
            RunName = "LHC10c";
            
        }
        RunList = LHC10c;
        break;
    case 2:
        if ( MCFlag )
        {
            RunYear = "2014";
            RunName = "LHC14j4d";
        }
        else
        {
            RunYear = "2010";
            RunName = "LHC10d";
        }
        RunList = LHC10d;
        break;
    case 3:
        if ( MCFlag )
        {
            RunYear = "2014";
            RunName = "LHC14j4e";
        }
        else
        {
            RunYear = "2010";
            RunName = "LHC10e";
        }
        RunList = LHC10e;
                
        break;
    default:
        cout << "[info]: Default Period ENABLED" << endl;
        if ( MCFlag )
        {
            RunYear = "2014";
            RunName = "LHC14j4b";
        }
        else
        {
            RunYear = "2010";
            RunName = "LHC10b";
        }
        RunList = LHC10b;
        break;
    }
    cout << "[info]: " << RunName << " Period chose" << endl;
    for ( int iRun = 0; iRun < RunList.size(); iRun++ )
    {
        cout << "[info]: Run: " << RunList[iRun] << " ADDED" << endl;
    }
    
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter                ->ProcessLine(".include $ROOTSYS/include");
    gInterpreter                ->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT                       ->ProcessLine(".include $ROOTSYS/include");
    gROOT                       ->ProcessLine(".include $ALICE_ROOT/include");
#endif
     
    // create the analysis manager
    AliAnalysisManager *mgr     = new AliAnalysisManager("AnalysisTaskExample");
    AliAODInputHandler *aodH    = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);


    // AnalysisTaskMyTask
    // PID Response
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter                ->LoadMacro("AliAnalysisTaskPhiCount.cxx++g");
    
    // PID Task
    TMacro PIDadd(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"));
    AliAnalysisTaskPIDResponse *PIDk;
    if ( MCFlag )   PIDk = reinterpret_cast<AliAnalysisTaskPIDResponse*>(PIDadd.Exec("kTRUE,kTRUE,kTRUE,4,kFALSE,\"\",kFALSE,kFALSE"));
    else            PIDk = reinterpret_cast<AliAnalysisTaskPIDResponse*>(PIDadd.Exec("kFALSE,kTRUE,kTRUE,4,kFALSE,\"\",kFALSE,kFALSE"));
    
    // Multiplicty Task
    TMacro MLTadd(gSystem->ExpandPathName("$ALICE PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"));
    AliMultSelectionTask *MLTk;
    MLTk = reinterpret_cast<AliMultSelectionTask*>(MLTadd.Exec());
    
    // Custom analysis task
    AliAnalysisTaskPhiCount *task;
    task = reinterpret_cast<AliAnalysisTaskPhiCount*>(gInterpreter->ExecuteMacro(Form("AddMyTask.C(%d,%d,%d)",MCFlag,PhiFlag,KaonFlag)));
#else
    gROOT                       ->LoadMacro("AliAnalysisTaskPhiCount.cxx++g");
    gROOT                       ->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    gROOT                       ->LoadMacro("AddMyTask.C");
    AddTaskPIDResponse();
    AliAnalysisTaskPhiCount *task = AddMyTask(MCFlag,PhiFlag,KaonFlag);
    
#endif

    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);

    if(local)
    {
        // if you want to run locally, we need to define some input
        TChain* chain = new TChain("aodTree");
        // add a few files to the chain (change this so that your local files are added)
        if ( MCFlag )   chain->Add("AliAOD_MC.root");
        else            chain->Add("AliAOD.root");
        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chain);
    }
    else
    {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        // make sure your source files get copied to grid
        alienHandler->SetAdditionalLibs("AliAnalysisTaskPhiCount.cxx AliAnalysisTaskPhiCount.h");
        alienHandler->SetAnalysisSource("AliAnalysisTaskPhiCount.cxx");
        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion("vAN-20200302_ROOT6-1");
        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");
        // select the input data
        if ( MCFlag )   alienHandler->SetGridDataDir(Form("/alice/sim/%s/%s",RunYear,RunName));
        else            alienHandler->SetGridDataDir(Form("/alice/data/%s/%s",RunYear,RunName));
        if ( MCFlag )   alienHandler->SetDataPattern("*AOD222/*AOD.root");
        else            alienHandler->SetDataPattern("*pass4/AOD221/*AOD.root");
        // MC has no prefix, data has prefix 000
        if ( !MCFlag ) alienHandler->SetRunPrefix("000");
        // runnumber
        for ( int iRun = 0; iRun < RunList.size(); iRun++ )
        {
            alienHandler->AddRunNumber(RunList[iRun]);
        }
        
        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(40);
        alienHandler->SetExecutable("myTask.sh");
        // specify how many seconds your job may take
        alienHandler->SetTTL(20000);
        alienHandler->SetJDLName("myTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate") 
        // (see below) mode, set SetMergeViaJDL(kFALSE) 
        // to collect final results
        alienHandler->SetMaxMergeStages(1);
        alienHandler->SetMergeViaJDL(kTRUE);
        //alienHandler->SetMergeViaJDL(kFALSE);

        // define the output folders
        alienHandler->SetGridWorkingDir(Form("%s",RunName));
        alienHandler->SetGridOutputDir(Form("%s",RunName));
        
        // Submit policy
        //alienHandler->SetUseSubmitPolicy();

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);
        
        if(gridTest)
        {
            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        }
        else
        {
            // else launch the full grid analysis
            alienHandler->SetRunMode("full");
            //alienHandler->SetRunMode("terminate");
            mgr->StartAnalysis("grid");
        }
    }
}
 
