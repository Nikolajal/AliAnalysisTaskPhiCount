// !TODO LIST
// TODO: Implement Rec and Phi for the KaonCouples
// TODO: Implement Rec for the true Phi
// TODO: Implement MC Flag


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
fAOD(0), fMCD(0), fOutputList(0), fOutputTree_SIG(0), fOutputTree_TRU(0), fHistVertex0(0), fHistVertex1(0), fHistTPCPID0(0), fHistTPCPID1(0), fHistTPCPID2(0), fHistTOFPID0(0), fHistTOFPID1(0), fHistTOFPID2(0), fPIDResponse(0), AODMCTrackArray(0)
{
    
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::AliAnalysisTaskPhiCount(const char* name) : AliAnalysisTaskSE(name),
   fAOD(0), fMCD(0), fOutputList(0), fOutputTree_SIG(0), fOutputTree_TRU(0), fHistVertex0(0), fHistVertex1(0), fHistTPCPID0(0), fHistTPCPID1(0), fHistTPCPID2(0), fHistTOFPID0(0), fHistTOFPID1(0), fHistTOFPID2(0), fPIDResponse(0), AODMCTrackArray(0)
{
    // Define Input
    DefineInput(0, TChain::Class());
    
    // Define Output
    DefineOutput(1, TList::Class());
    DefineOutput(2, TTree::Class());
    DefineOutput(3, TTree::Class());
}

//_____________________________________________________________________________

AliAnalysisTaskPhiCount::~AliAnalysisTaskPhiCount()
{
    if(fOutputList)
    {
        delete fOutputList;
    }
    if(fOutputTree_SIG)
    {
        delete fOutputTree_SIG;
    }
    if(fOutputTree_TRU)
    {
        delete fOutputTree_TRU;
    }
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::UserCreateOutputObjects()
{
    fOutputTree_SIG = new TTree ("SIG_Kaon_Tree"    ,"Real Data Tree for Kaon+- Couples");
    fOutputTree_SIG->Branch     ("nKaonCouple"      ,&fnKaonCouple,     "fnKaonCouple/I");
    fOutputTree_SIG->Branch     ("iKaon"            ,&fiKaon,           "fiKaon[fnKaonCouple]/I");
    fOutputTree_SIG->Branch     ("jKaon"            ,&fjKaon,           "fjKaon[fnKaonCouple]/I");
    fOutputTree_SIG->Branch     ("InvMass"          ,&fInvMass,         "fInvMass[fnKaonCouple]/F");
    fOutputTree_SIG->Branch     ("bEta"             ,&fKbEta,           "fKbEta[fnKaonCouple]/O");
    fOutputTree_SIG->Branch     ("pT"               ,&fKpT,             "fKpT[fnKaonCouple]/F");
    if ( kMCbool ) fOutputTree_SIG->Branch     ("bPhi"             ,&fKbPhi,           "fKbPhi[fnKaonCouple]/O");
    
    PostData(2, fOutputTree_SIG);
    
    fOutputTree_TRU = new TTree ("TRU_Phi__Tree",   "A ROOT tree for pythia MC - Phi");
    fOutputTree_TRU->Branch     ("nPhi",            &fnPhi,            "fnPhi/I");
    fOutputTree_TRU->Branch     ("bEta",            &fPbEta,           "fPbEta[fnPhi]/O");
    fOutputTree_TRU->Branch     ("bRec",            &fPbRec,           "fPbRec[fnPhi]/O");
    fOutputTree_TRU->Branch     ("bKdc",            &fPbKdc,           "fPbKdc[fnPhi]/O");
    fOutputTree_TRU->Branch     ("pT",              &fPpT,             "fPpT[fnPhi]/F");
    
    if ( kMCbool ) PostData(3, fOutputTree_TRU);
    
    fOutputList     = new TList();
    fOutputList     ->SetOwner(kTRUE);
    fHistVertex0    = new TH1F("fHistVertex0", "Collision Vertex (FULL)", 1000, -15, 15);
    fHistVertex1    = new TH1F("fHistVertex1", "Collision Vertex (CUTS)", 1000, -15, 15);
    fHistTPCPID0    = new TH2F("fHistTPCPID0", "TPC Response (ALL)"     , 100, 0, 4, 100, 0, 200);
    fHistTPCPID1    = new TH2F("fHistTPCPID1", "TPC Response (Sel1)"    , 100, 0, 4, 100, 0, 200);
    fHistTPCPID2    = new TH2F("fHistTPCPID2", "TPC Response (Sel2)"    , 100, 0, 4, 100, 0, 200);
    fHistTPCPID3    = new TH2F("fHistTPCPID3", "TPC Response (Sel3)"    , 100, 0, 4, 200, -10, 10);
    fHistTOFPID0    = new TH2F("fHistTOFPID0", "TOF Response (ALL)"     , 100, 0, 4, 100, 0, 1.2);
    fHistTOFPID1    = new TH2F("fHistTOFPID1", "TOF Response (Sel1)"    , 100, 0, 4, 100, 0, 1.2);
    fHistTOFPID2    = new TH2F("fHistTOFPID2", "TOF Response (Sel2)"    , 100, 0, 4, 100, 0, 1.2);
    fHistTOFPID3    = new TH2F("fHistTOFPID3", "TOF Response (Sel3)"    , 100, 0, 4, 200, -10, 10);
    fOutputList->Add(fHistVertex0);
    fOutputList->Add(fHistTPCPID0);
    fOutputList->Add(fHistTOFPID0);
    fOutputList->Add(fHistVertex1);
    fOutputList->Add(fHistTPCPID1);
    fOutputList->Add(fHistTOFPID1);
    fOutputList->Add(fHistTPCPID2);
    fOutputList->Add(fHistTOFPID2);
    fOutputList->Add(fHistTPCPID3);
    fOutputList->Add(fHistTOFPID3);
    
    PostData(1, fOutputList);
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsPrimaryVertexCandidate ( AliAODEvent* event )
{
    // Recovering Primary Vertex from General methods and SPD
    auto    PrimaryVertexSPD    = fAOD->GetPrimaryVertexSPD();
    auto    PrimaryVertexTRK    = fAOD->GetPrimaryVertex();
            fPrimaryVertex      = PrimaryVertexTRK;
    
    // Requires the vertex is reconstructed by the SPD
    if ( !PrimaryVertexSPD  ||  PrimaryVertexSPD->GetNContributors() < 1 ) return false;

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
        if ( std::fabs(VertexZSPD-VertexZTRK) > 0.5 ) return false;
    }
    
    // Fill the Vertex Z position histogram
    fHistVertex0->Fill(fPrimaryVertex->GetZ());
    
    if ( std::fabs(fPrimaryVertex->GetZ()) > 10. ) return false;
    
    // Fill the Vertex Z position histogram
    fHistVertex1->Fill(fPrimaryVertex->GetZ());
    
    return true;
}

//_____________________________________________________________________________

bool AliAnalysisTaskPhiCount::fIsKaonCandidate ( AliAODTrack* track )
{
    // Check the track is there and has proper kinematics
    if ( !track                         || !track->TestFilterBit(5)         ) return false;
    if (  std::fabs(track->Pt()) < 0.15 ||  std::fabs(track->Eta()) > 0.80  ) return false;
    
    fFillPIDHist(track,0);
    fFillPIDHist(track,3);
    
    // Check the PID is present and within desired parameters
    if ( !fPIDResponse ) return false;
    auto fbTPC       = (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTPC, track) == AliPIDResponse::kDetPidOk);
    auto fbTOF       = (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF, track) == AliPIDResponse::kDetPidOk);
    auto ffSigTOF    = std::fabs(fPIDResponse->NumberOfSigmasTOF(track,AliPID::kKaon));
    auto ffSigTPC    = std::fabs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kKaon));
    
    /*  STANDARD
    if ( !fbTPC || ffSigTPC > 5. ) return false;
    if (  fbTOF && ffSigTOF < 3. )
    {
        fFillPIDHist(track,1);
        return true;
    }
    if ( !fbTOF && ffSigTPC < 3. )
    {
        fFillPIDHist(track,2);
        return true;
    }
    return false;
     */
    
    /*   PAPER

        if (    !fbTPC      ||  ffSigTPC > 5.   )   return false;
        if (    fbTOF       &&  ffSigTOF > 3.   )   return false;
        if (    track->P()   >=  0.35 && ffSigTPC    >   3.  )   return false;
        return true;
*/

    /*  CUSTOM */
    if ( !fbTPC || (fbTOF && ffSigTOF > 3) )      return false;
    if ( track->Pt() >= 0.28 &&  fbTOF && ffSigTPC > 5. )   return false;
    if ( track->Pt() >= 0.28 && !fbTOF && ffSigTPC > 3. )   return false;
    fFillPIDHist(track,1);
    if ( track->Pt() <  0.28  && track->Pt() >=  0.24  && ffSigTPC > 6. )    return false;
    if ( track->Pt() <  0.24  && track->Pt() >=  0.16  && ffSigTPC > 7. )    return false;
    if ( track->Pt() <  0.16  && track->Pt() >=  0.00  && ffSigTPC > 7.5 )    return false;
    fFillPIDHist(track,2);
    return true;
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
    
    // Check the event is there and has a primary vertex with due requirements
    if ( !fIsPrimaryVertexCandidate(dynamic_cast<AliAODEvent*>(InputEvent())) ) return;
     
    // Define and Fetch PID with Manager
    AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
    if (man)
    {
        AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
        if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
    }
    
    // Utility variables
    Int_t           nTrack(fAOD->GetNumberOfTracks());
                    fnKaon      = 0;
                    fnPhi       = 0;
                    fnKaonCouple= 0;
    Int_t           nKaon1_sign, nKaon2_sign;
    TLorentzVector  fKaon1, fKaon2, fPhi;
    
    // Looping over tracks
    for (   Int_t iTrack(0); iTrack < nTrack; iTrack++ )
    {
        // Protection for segmentation fault due to exceeding array size
        if ( fnKaon >= 1024 ) break;
        
        // Check the track is a Kaon and has due requirements
        if ( !fIsKaonCandidate(static_cast<AliAODTrack*>(fAOD->GetTrack(iTrack))) ) continue;
        
        // Storing for later recovery the track ID
        faKaon[fnKaon] = iTrack;
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

    // Discarding the event with over 1024 Kaons
    if ( fnKaon <= 1024 && fnKaonCouple <= 1024 ) fOutputTree_SIG -> Fill();
    if ( kMCbool        && fnPhi <= 1024 ) PostData(3, fOutputTree_TRU);
    if ( kMCbool ) fOutputTree_TRU -> Fill();
    PostData(2, fOutputTree_SIG);
    PostData(1, fOutputList);
}

//_____________________________________________________________________________

void AliAnalysisTaskPhiCount::Terminate(Option_t *)
{

}

//_____________________________________________________________________________
