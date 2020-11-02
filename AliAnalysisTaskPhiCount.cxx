// !TODO LIST
// TODO: You're all set!


#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "TTree.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAODMCParticle.h"
#include "AliPIDResponse.h"
#include "AliAnalysisTaskPhiCount.h"

class AliAnalysisTaskPhiCount;
class AliPIDResponse;
 
using namespace std;

ClassImp(AliAnalysisTaskPhiCount)

AliAnalysisTaskPhiCount::AliAnalysisTaskPhiCount() : AliAnalysisTaskSE(),
fAOD(0), fMCD(0), fAnalysisOutputList(0), fQCOutputList(0), fPhiCandidate(0), fPhiEfficiency(0), fHistVertex0(0), fHistVertex1(0), fHistTPCPID0(0), fHistTPCPID1(0), fHistTPCPID2(0), fHistTOFPID0(0), fHistTOFPID1(0), fHistTOFPID2(0), fPIDResponse(0), AODMCTrackArray(0)
{
    
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::AliAnalysisTaskPhiCount(const char* name) : AliAnalysisTaskSE(name),
   fAOD(0), fMCD(0), fAnalysisOutputList(0), fQCOutputList(0), fPhiCandidate(0), fPhiEfficiency(0), fHistVertex0(0), fHistVertex1(0), fHistTPCPID0(0), fHistTPCPID1(0), fHistTPCPID2(0), fHistTOFPID0(0), fHistTOFPID1(0), fHistTOFPID2(0), fPIDResponse(0), AODMCTrackArray(0)
{
    // Define Input
    DefineInput(0, TChain::Class());
    
    // Define Output
    DefineOutput(1, TList::Class());
    DefineOutput(2, TList::Class());
    DefineOutput(3, TTree::Class());
    DefineOutput(4, TTree::Class());
    DefineOutput(5, TTree::Class());
    DefineOutput(6, TTree::Class());
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::~AliAnalysisTaskPhiCount()
{
    // Deleting TLists
    if( fAnalysisOutputList )
    {
        delete fAnalysisOutputList;
    }
    if( fQCOutputList )
    {
        delete fQCOutputList;
    }
    if( fKaonCandidate )
    {
        delete fKaonCandidate;
    }
    if( fPhiCandidate )
    {
        delete fPhiCandidate;
    }
    if( fKaonEfficiency )
    {
        delete fKaonEfficiency;
    }
    if( fPhiEfficiency )
    {
        delete fPhiEfficiency;
    }
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::UserCreateOutputObjects()
{
    // Various utility Histograms TList initialisation
    fAnalysisOutputList     = new TList();
    fAnalysisOutputList     ->SetOwner(kTRUE);
    fHistVertex0    = new TH1F("fHistVertex0", "Collision Vertex (FULL)", 1000, -15, 15);
    fHistVertex1    = new TH1F("fHistVertex1", "Collision Vertex (CUTS)", 1000, -15, 15);
    fHistTPCPID0    = new TH2F("fHistTPCPID0", "TPC Response (ALL)"     , 100, 0, 4, 200, 0, 400);
    fHistTPCPID1    = new TH2F("fHistTPCPID1", "TPC Response (Sel1)"    , 100, 0, 4, 200, 0, 400);
    fHistTPCPID2    = new TH2F("fHistTPCPID2", "TPC Response (Sel2)"    , 100, 0, 4, 200, 0, 400);
    fHistTOFPID0    = new TH2F("fHistTOFPID0", "TOF Response (ALL)"     , 100, 0, 4, 100, 0, 1.2);
    fHistTOFPID1    = new TH2F("fHistTOFPID1", "TOF Response (Sel1)"    , 100, 0, 4, 100, 0, 1.2);
    fHistTOFPID2    = new TH2F("fHistTOFPID2", "TOF Response (Sel2)"    , 100, 0, 4, 100, 0, 1.2);
    fHistEvntEff    = new TH1F("fHistEvntEff", "fHistEvntEff"           , 4,   0.5, 4.5);
    fAnalysisOutputList->Add(fHistEvntEff);
    fAnalysisOutputList->Add(fHistVertex0);
    fAnalysisOutputList->Add(fHistTPCPID0);
    fAnalysisOutputList->Add(fHistTOFPID0);
    fAnalysisOutputList->Add(fHistVertex1);
    fAnalysisOutputList->Add(fHistTPCPID1);
    fAnalysisOutputList->Add(fHistTOFPID1);
    fAnalysisOutputList->Add(fHistTPCPID2);
    fAnalysisOutputList->Add(fHistTOFPID2);
    
    PostData(1, fAnalysisOutputList);
    
    // QC utility Histograms TList initialisation
    fQCOutputList     = new TList();
    fQCOutputList     ->SetOwner(kTRUE);
    fHistTPCPID3    = new TH2F("fHistTPCPID3", "TPC Response (Sel3)"    , 100, 0, 4, 200, -10, 10);
    fHistTOFPID3    = new TH2F("fHistTOFPID3", "TOF Response (Sel3)"    , 100, 0, 4, 200, -10, 10);
    fQCOutputList->Add(fHistTPCPID3);
    fQCOutputList->Add(fHistTOFPID3);
    
    PostData(2, fQCOutputList);
    
    // PhiCandidate Tree Set-Up
    fPhiCandidate = new TTree   ("PhiCandidate",    "Data Tree for Phi Candidates");
    fPhiCandidate->Branch       ("fMultiplicity",   &fMultiplicity,     "fMultiplicity/F");
    fPhiCandidate->Branch       ("nPhi",            &fnPhi,             "fnPhi/I");
    fPhiCandidate->Branch       ("Px",              &fPhiPx,            "fPhiPx[fnPhi]/F");
    fPhiCandidate->Branch       ("Py",              &fPhiPy,            "fPhiPy[fnPhi]/F");
    fPhiCandidate->Branch       ("Pz",              &fPhiPz,            "fPhiPz[fnPhi]/F");
    fPhiCandidate->Branch       ("InvMass",         &fInvMass,          "fInvMass[fnPhi]/F");
    fPhiCandidate->Branch       ("iKaon",           &fiKaon,            "fiKaon[fnPhi]/I");
    fPhiCandidate->Branch       ("jKaon",           &fjKaon,            "fjKaon[fnPhi]/I");
    
    if ( kPhibool )                 PostData(3, fPhiCandidate);
    
    // KaonCandidate Tree Set-Up
    fKaonCandidate = new TTree ("KaonCandidate",    "Data Tree for Kaon Candidates");
    fKaonCandidate->Branch     ("fMultiplicity",    &fMultiplicity,     "fMultiplicity/F");
    fKaonCandidate->Branch     ("fnKaon",           &fnKaon,            "fnKaon/I");
    fKaonCandidate->Branch     ("Px",               &fKaonPx,           "fKaonPx[fnKaon]/F");
    fKaonCandidate->Branch     ("Py",               &fKaonPy,           "fKaonPy[fnKaon]/F");
    fKaonCandidate->Branch     ("Pz",               &fKaonPz,           "fKaonPz[fnKaon]/F");
    
    if ( kKaonbool )                PostData(4, fKaonCandidate);

    fPhiEfficiency = new TTree  ("PhiEfficiency",   "MC Tree for Phi Efficiency");
    fPhiEfficiency->Branch      ("nPhi",            &fnPhiTru,          "fnPhi/I");
    fPhiEfficiency->Branch      ("bEta",            &fPbEta,            "fPbEta[fnPhi]/O");
    fPhiEfficiency->Branch      ("bRec",            &fPbRec,            "fPbRec[fnPhi]/O");
    fPhiEfficiency->Branch      ("bKdc",            &fPbKdc,            "fPbKdc[fnPhi]/O");
    fPhiEfficiency->Branch      ("pT",              &fPpT,              "fPpT[fnPhi]/F");
    
    if ( kPhibool   &&  kMCbool )   PostData(5, fPhiEfficiency);
    
    fKaonEfficiency = new TTree ("KaonEfficiency",   "MC Tree for Kaon Efficiency");
    
    if ( kKaonbool  &&  kMCbool )   PostData(6, fKaonEfficiency);
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::fSetZero()
{
    //Setting all counters and global variables to zero
    fMultiplicity   =   0;
    fnPhi           =   0;
    fnKaon          =   0;
    fnPhiTru        =   0;
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::fPostData()
{
    // Setting postdata options
    
    // Post-data for TLists
    PostData(1, fAnalysisOutputList);
    PostData(2, fQCOutputList);
    
    // Discarding events with over 1024 Kaons
    if ( fnKaon <= 1024 && fnPhi <= 1024 )  fPhiCandidate   ->  Fill();
    if ( fnKaon <= 1024 )                   fKaonCandidate  ->  Fill();
    if ( fnPhiTru <= 1024 )                 fPhiEfficiency  ->  Fill();
                                            fKaonEfficiency ->  Fill();
    
    // Post-data for TTrees
    if ( kPhibool )                 PostData(3, fPhiCandidate);
    if ( kKaonbool )                PostData(4, fKaonCandidate);
    if ( kPhibool   &&  kMCbool )   PostData(5, fPhiEfficiency);
    if ( kKaonbool  &&  kMCbool )   PostData(6, fKaonEfficiency);
    
    
    if ( fnKaon <= 1024 && fnPhi <= 1024 ) fPhiCandidate -> Fill();
    if ( fnPhiTru <= 1024 ) fPhiEfficiency -> Fill();
    if ( kMCbool ) PostData(4, fPhiEfficiency);
    PostData(3, fPhiCandidate);
    
    
    fPhiCandidate -> Fill();
    PostData(3, fPhiCandidate);
    if ( kMCbool ) fPhiEfficiency -> Fill();
    if ( kMCbool ) PostData(4, fPhiEfficiency);
    fFillVtxHist(2);
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPrimaryVertexCandidate ( AliAODEvent* event )
{
    // Recovering Primary Vertex from General methods and SPD
    auto    PrimaryVertexSPD    = fAOD->GetPrimaryVertexSPD();
    auto    PrimaryVertexTRK    = fAOD->GetPrimaryVertex();
            fPrimaryVertex      = PrimaryVertexTRK;
    
    // Requires the vertex is reconstructed by the SPD
    if ( !PrimaryVertexSPD  ||  PrimaryVertexSPD->GetNContributors() < 1 )
    {
        fFillVtxHist(1);
        fPostData();
        return false;
    }

    // In lack of the general Method reconstructed Vertex, take the SPD reconstruction
    if ( !PrimaryVertexTRK  ||  PrimaryVertexTRK->GetNContributors() < 1 )
    {
        fPrimaryVertex = PrimaryVertexSPD;
    }
    
    // If Both are present and reconstructed discard event if the two mismatch for more than 0.5cm
    else
    {
        auto VertexZSPD = PrimaryVertexSPD->GetZ();
        auto VertexZTRK = PrimaryVertexTRK->GetZ();
        if ( std::fabs(VertexZSPD-VertexZTRK) > 0.5 )
        {
            fFillVtxHist(2);
            fPostData();
            return false;
        }
    }
    
    // Fill the Vertex Z position histogram
    fHistVertex0->Fill(fPrimaryVertex->GetZ());
    
    if ( std::fabs(fPrimaryVertex->GetZ()) > 10. )
    {
        fFillVtxHist(3);
        fPostData();
        return false;
    }
    
    // Fill the Vertex Z position histogram
    fHistVertex1->Fill(fPrimaryVertex->GetZ());
    
    return true;
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsTrackCandidate ( AliAODTrack* track )
{
    // Check the track is there and has proper kinematics
    if ( !track                         || !track->TestFilterBit(5)         ) return false;
    if (  std::fabs(track->Pt()) < 0.15 ||  std::fabs(track->Eta()) > 0.80  ) return false;
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::fSetKaonPID ( AliAODTrack* track )
{
    fFillPIDHist(track,0);
    fFillPIDHist(track,3);
    
    // Check the PID is present and within desired parameters
    if ( !fPIDResponse ) return false;
    auto fbTPC       = (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTPC, track) == AliPIDResponse::kDetPidOk);
    auto fbTOF       = (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF, track) == AliPIDResponse::kDetPidOk);
    auto ffSigTOF    = std::fabs(fPIDResponse->NumberOfSigmasTOF(track,AliPID::kKaon));
    auto ffSigTPC    = std::fabs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kKaon));
    
    fTOFSigma[fnKaon]= static_cast<Int_t>(ffSigTOF*10);
    fTPCSigma[fnKaon]= static_cast<Int_t>(ffSigTPC*10);
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsKaonCandidate ( AliAODTrack* track )
{
    /*  CUSTOM
    if ( !fbTPC || (fbTOF && ffSigTOF > 3) )      return false;
    if ( track->Pt() >= 0.28 &&  fbTOF && ffSigTPC > 5. )   return false;
    if ( track->Pt() >= 0.28 && !fbTOF && ffSigTPC > 3. )   return false;
    fFillPIDHist(track,1);
    if ( track->Pt() <  0.28  && track->Pt() >=  0.24  && ffSigTPC > 6. )    return false;
    if ( track->Pt() <  0.24  && track->Pt() >=  0.16  && ffSigTPC > 7. )    return false;
    if ( track->Pt() <  0.16  && track->Pt() >=  0.00  && ffSigTPC > 7.5 )    return false;
    fFillPIDHist(track,2);
    return true;
     */
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsKaonTruPhi ( AliAODMCParticle* piKaon, AliAODMCParticle* pjKaon )
{
    if ( !piKaon || !pjKaon ) return false;
    if ( piKaon->GetMother() == pjKaon->GetMother() && (static_cast<AliAODMCParticle*>(AODMCTrackArray->At(pjKaon->GetMother()))->GetPdgCode() == 333 ) ) return true;
    else return false;
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPhiCandidate ( TLorentzVector fPhi )
{
    if ( fPhi.Mag() < 0.75 || fPhi.Mag() > 1.25 ) return false;
    return true;
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPhiGen ( AliAODMCParticle* particle )
{
    if ( particle->GetNDaughters() != 2 ) return false;
    auto const Dau1                 =   static_cast<AliAODMCParticle*>  (AODMCTrackArray->At(particle->GetDaughterFirst()));
    auto const Dau2                 =   static_cast<AliAODMCParticle*>  (AODMCTrackArray->At(particle->GetDaughterLast()));
    
    if ( !Dau1 || !Dau2 ) return false;
    
    return ( ( particle->GetNDaughters() == 2 ) && ( Dau1->GetPdgCode() == -Dau2->GetPdgCode() ) && ( abs(Dau1->GetPdgCode()) == 321 ) );
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPhiRec ( AliAODMCParticle* particle )
{
    // To be recrodable, it must come from a K^+k^- decay
    if ( !fIsPhiGen(particle) ) return false;
    
    // Generating Daughter 1 and 2 Particles instances
    auto const Dau1                 =   static_cast<AliAODMCParticle*>  (AODMCTrackArray->At(particle->GetDaughterFirst()));
    auto const Dau2                 =   static_cast<AliAODMCParticle*>  (AODMCTrackArray->At(particle->GetDaughterLast()));
    
    // Generating a bool check
    bool fCheckDau1                 =   false;
    bool fCheckDau2                 =   false;
    
    // looping over all kaons
    for ( int iKaon = 0; iKaon < fnKaon; iKaon++ )
    {
        // recovering kaon track object
        auto track  =   static_cast<AliAODTrack*>(fAOD->GetTrack(faKaon[iKaon]));
        
        // Sanity check it is a kaon
        if ( abs((static_cast<AliAODMCParticle*>(AODMCTrackArray->At(abs(track->GetLabel()))))->GetPdgCode()) != 321 ) continue;
        
        
        if ( abs(track->GetLabel()) == Dau1->GetLabel() ) fCheckDau1 = true;
        if ( abs(track->GetLabel()) == Dau2->GetLabel() ) fCheckDau2 = true;
    }
    return fCheckDau1 && fCheckDau2;
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPhiValid ( AliAODMCParticle* particle )
{
    if ( !particle ) return false;
    if ( particle->GetPdgCode() == 333 ) return true;
    else return false;
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::fFillPIDHist ( AliAODTrack * track , Int_t iIndex )
{
    if ( !track ) return;
    if ( (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTPC, track) == AliPIDResponse::kDetPidOk) )
    {
        auto ffSigTPC    = fPIDResponse->NumberOfSigmasTPC(track,AliPID::kKaon);
        if ( iIndex == 0 ) fHistTPCPID0->Fill(track->P(), track->GetTPCsignal());
        if ( iIndex == 1 ) fHistTPCPID1->Fill(track->P(), track->GetTPCsignal());
        if ( iIndex == 2 ) fHistTPCPID2->Fill(track->P(), track->GetTPCsignal());
        if ( iIndex == 3 ) fHistTPCPID3->Fill(track->Pt(), ffSigTPC);
    }
    if ( (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF, track) == AliPIDResponse::kDetPidOk) )
    {
        auto ffSigTOF    = fPIDResponse->NumberOfSigmasTOF(track,AliPID::kKaon);
        
        Float_t fTrackLength(track->GetIntegratedLength()*1e-2);   // Track Length in cm
        if ( fTrackLength < 0. ) return;
        
        Float_t fTrackTime(track->GetTOFsignal()*1e-12);           // Track Time in s
        if ( fTrackTime < 0. ) return;
        
        // Track Beta
        Float_t fTrackBeta = fTrackLength/(fTrackTime*TMath::C());
        
        if ( iIndex == 0 ) fHistTOFPID0->Fill(track->P(), fTrackBeta);
        if ( iIndex == 1 ) fHistTOFPID1->Fill(track->P(), fTrackBeta);
        if ( iIndex == 2 ) fHistTOFPID2->Fill(track->P(), fTrackBeta);
        if ( iIndex == 3 ) fHistTOFPID3->Fill(track->Pt(), ffSigTOF);
    }
    return;
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::fFillVtxHist ( Int_t iIndex )
{
    for ( Int_t iFill = 1; iFill <= iIndex; iFill++ )
    {
        fHistEvntEff->Fill(iFill);
    }
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::UserExec(Option_t *)
{
    // Recovering Event Data
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
    fMCD = dynamic_cast<AliMCEvent*>(MCEvent());
    
    // Check the event is there
    if ( !fAOD ) return;
    if ( !fMCD && kMCbool ) return;
    
    // Recover the MC tracks
    if ( kMCbool )      AODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
    if ( !AODMCTrackArray && kMCbool )     return;
    
    // Setting zero all counters and global variables, setting utility variables
    fSetZero()
    Int_t           nTrack(fAOD->GetNumberOfTracks());
    Int_t           nKaon1_sign, nKaon2_sign;
    TLorentzVector  fKaon1, fKaon2, fPhi;
    
    // Check the event is there and has a primary vertex with due requirements
    if ( !fIsPrimaryVertexCandidate(dynamic_cast<AliAODEvent*>(InputEvent())) ) return;
     
    // Define and Fetch PID with Manager
    AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
    if (man)
    {
        AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
        if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
    }
    
    // Looping over tracks
    for (   Int_t iTrack(0); iTrack < nTrack; iTrack++ )
    {
        // Protection for segmentation fault due to exceeding array size
        if ( fnKaon >= 1024 ) break;
        
        // Recovering Track
        auto    fCurrent_Track  =   static_cast<AliAODTrack*>(fAOD->GetTrack(iTrack));
        
        // Check the track has due requirements
        if ( !fIsTrackCandidate(fCurrent_Track) ) continue;
        
        // Filling the Kaon Tree
        fSetKaonPID(fCurrent_Track);
        
        fnKaon++;
    }
       
    //Coupling Kaons
    for ( Int_t iKaon(0); iKaon < fnKaon; iKaon++)
    {
        // Discarding the event with over 1024 Kaons
        if ( fnKaon >= 1024 ) break;
        
        // Storing first Kaon kinematics and sign
        AliAODTrack* itrack = static_cast<AliAODTrack*>(fAOD->GetTrack(faKaon[iKaon]));
        fKaon1.SetXYZM(itrack->Px(),itrack->Py(),itrack->Pz(),.493677);
        nKaon1_sign = (fAOD->GetTrack(faKaon[iKaon]))->GetSign();
        
        for ( Int_t jKaon(iKaon+1); jKaon < fnKaon; jKaon++)
        {
            // Protection for segmentation fault due to exceeding array size
            if ( fnKaonCouple >= 1024 ) break;
            
            //Storing second Kaon Sign
            nKaon2_sign = (fAOD->GetTrack(faKaon[jKaon]))->GetSign();
            
            if ( nKaon1_sign == nKaon2_sign ) continue;
            
            // Storing second Kaon Kinematics and combining the two
            AliAODTrack* jtrack = static_cast<AliAODTrack*>(fAOD->GetTrack(faKaon[jKaon]));
            fKaon2.SetXYZM(jtrack->Px(),jtrack->Py(),jtrack->Pz(),.493677);
            
            // Check the Phi is a good candidate
            fPhi = (fKaon1 + fKaon2);
            if ( !fIsPhiCandidate(fPhi) ) continue;
            
            fKbEta[fnKaonCouple]        = (std::fabs(fPhi.Rapidity()) <= 0.5);
            fiKaon[fnKaonCouple]        = iKaon;
            fjKaon[fnKaonCouple]        = jKaon;
            fInvMass[fnKaonCouple]      = (fPhi).Mag();
            fKpT[fnKaonCouple]          = (fPhi).Pt();
            
            if ( kMCbool )  //  -   -   -   -   -   -   -   To implement
            {
                auto fParticleiKaon = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(abs(itrack->GetLabel())));
                auto fParticlejKaon = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(abs(jtrack->GetLabel())));
                fKbPhi[fnKaonCouple]        = fIsKaonTruPhi( fParticleiKaon, fParticlejKaon );
            }
            
            fnKaonCouple++;
        }
    }
    
    // Loop over all primary MC particle
    if ( kMCbool )
    {
        for(Long_t i = 0; i < AODMCTrackArray->GetEntriesFast(); i++)
        {
            if ( fnPhi >= 1024 ) break;
            
            AliAODMCParticle* particle = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(i));
            
            if ( !fIsPhiValid( particle ) ) continue;
            
            // Kinematics
            fPbEta[fnPhi]               =   (fabs(particle->Y()) <= 0.5);
            fPpT[fnPhi]                 =   particle->Pt();
            
            // Gen
            fPbKdc[fnPhi]               =   fIsPhiGen( particle );
            
            // Rec
            fPbRec[fnPhi]               =   fIsPhiRec( particle );
                
            fnPhi++;
        }
    }
 
    // Saving output
    fFillVtxHist(4);
    fPostData();
    
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::Terminate(Option_t *)
{

}

//_____________________________________________________________________________
