// TODO LIST
// TODO: You're all set!

#ifndef AliAnalysisTaskPhiCount_H
#define AliAnalysisTaskPhiCount_H

#include "AliAnalysisTaskSE.h"

class AliPIDResponse;
class AliAODTrack;

class AliAnalysisTaskPhiCount : public AliAnalysisTaskSE
{
    public:
                                AliAnalysisTaskPhiCount     ( );
                                AliAnalysisTaskPhiCount     ( const char *      name );
        virtual                 ~AliAnalysisTaskPhiCount    ( );

        virtual void            UserCreateOutputObjects     ( );
        virtual void            UserExec                    ( Option_t*         option );
        virtual void            Terminate                   ( Option_t*         option );
        
        void                    fSetMCFlag                  ( Bool_t MCFlag )           { kMCbool = MCFlag; }
    
    private:
        bool                    fIsPrimaryVertexCandidate   ( AliAODEvent*      event );
        bool                    fIsKaonCandidate            ( AliAODTrack *     track );
        bool                    fIsPhiCandidate             ( TLorentzVector    fPhi );
        bool                    fIsPhiGen                   ( AliAODMCParticle* particle );
        bool                    fIsPhiRec                   ( AliAODMCParticle* particle );
        bool                    fIsPhiValid                 ( AliAODMCParticle* particle );
        bool                    fIsKaonTruPhi               ( AliAODMCParticle* piKaon, AliAODMCParticle* pjKaon );
        void                    fFillPIDHist                ( AliAODTrack *     track, Int_t iIndex );
        void                    fFillVtxHist                ( Int_t iIndex );
        AliAODVertex*           fGetPrimaryVertex           ( AliAODEvent*      event ) { return fIsPrimaryVertexCandidate(event) ? fPrimaryVertex : nullptr; };
    
        AliAODEvent*            fAOD;               //! input event
        AliMCEvent*             fMCD;               //! input event MC

        TClonesArray*           AODMCTrackArray;    //! MC Tracks Array

        Bool_t                  kMCbool;            // MC Flag
        
        // Tree
        TTree*                  fOutputTree_SIG;    //! output tree for Signal
        TTree*                  fOutputTree_TRU;    //! output tree for MC Truth
         
        // Tree Variables ( MC Truth )
        Int_t                   fnPhi;              //! Number of Phis produced found
        Bool_t                  fPbEta      [1024]; //! bEta
        Bool_t                  fPbRec      [1024]; //! bRec
        Bool_t                  fPbKdc      [1024]; //! bKdc
        Float_t                 fPpT        [1024]; //! Transverse Momentum
            
        // Tree Variables ( Signal )
        Int_t                   fnKaonCouple;       //! Number of Kaon Couples found
        Int_t                   fiKaon      [1024]; //! iKaon
        Int_t                   fjKaon      [1024]; //! jKaon
        Float_t                 fInvMass    [1024]; //! Invariant Mass
        Bool_t                  fKbEta      [1024]; //! bEta
        Int_t                   fKnRec      [1024]; //! bRec
        Bool_t                  fKbPhi      [1024]; //! bRec
        Float_t                 fKpT        [1024]; //! Transverse Momentum
    
        // Utility Variables
        Int_t                   fnKaon;
        Int_t                   faKaon      [1024];
        
        // List
        TList*                  fAnalysisOutputList;//! Analysis output list
        TList*                  fQCOutputList;      //! Analysis output list
        
        // List Variables
        TH1F*                   fHistEvntEff;       //! histogram of Event Efficiency
        TH1F*                   fHistVertex0;       //! histogram of Vertex
        TH2F*                   fHistTPCPID0;       //! histogram of TPC PID
        TH2F*                   fHistTOFPID0;       //! histogram of TOF PID
        TH1F*                   fHistVertex1;       //! histogram of Vertex
        TH2F*                   fHistTPCPID1;       //! histogram of TPC PID
        TH2F*                   fHistTOFPID1;       //! histogram of TOF PID
        TH2F*                   fHistTPCPID2;       //! histogram of TPC PID
        TH2F*                   fHistTOFPID2;       //! histogram of TOF PID
        TH2F*                   fHistTOFPID3;       //! histogram of TOF PID Sigma
        TH2F*                   fHistTPCPID3;       //! histogram of TPC PID Sigma

        AliAnalysisTaskPhiCount   (const AliAnalysisTaskPhiCount&); // not implemented
        AliAnalysisTaskPhiCount&  operator = (const AliAnalysisTaskPhiCount&); // not implemented
    
        AliPIDResponse*         fPIDResponse;       //! PID Response obj
    
        AliAODVertex*           fPrimaryVertex;     //! AOD Vertex obj
    
        ClassDef(AliAnalysisTaskPhiCount, 1);
};

#endif
