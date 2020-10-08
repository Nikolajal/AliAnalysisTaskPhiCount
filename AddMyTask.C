AliAnalysisTaskPhiCount* AddMyTask( Bool_t MCFlag, TString name = "name" )
{
    // Analysis Manager
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    
    // Filename
    TString fileName = AliAnalysisManager::GetCommonFileName();
    
    // Task initialisation
    AliAnalysisTaskPhiCount* task = new AliAnalysisTaskPhiCount(name.Data());
    
    // Checks
    if (!mgr)                           return 0x0;
    if (!mgr->GetInputEventHandler())   return 0x0;
    if (!task)                          return 0x0;
    
    // task Selection
    task->SelectCollisionCandidates(AliVEvent::kAnyINT);
    
    // MC option
    task -> fSetMCFlag(MCFlag);
    
    // Input
    mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());
    
    // Output
    mgr->ConnectOutput(task,2,mgr->CreateContainer("MyOutputContainerTreeSIG", TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    if ( MCFlag ) mgr->ConnectOutput(task,3,mgr->CreateContainer("MyOutputContainerTreeTRU", TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,1,mgr->CreateContainer("MyOutputContainerListUTL", TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    
    // Add task
    mgr->AddTask(task);
    
    return task;
}
