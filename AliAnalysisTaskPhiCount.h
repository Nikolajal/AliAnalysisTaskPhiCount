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
    // Constructors
                                AliAnalysisTaskPhiCount     ( );
                                AliAnalysisTaskPhiCount     ( const char *      name );
        virtual                 ~AliAnalysisTaskPhiCount    ( );

    // Required implementations
        virtual void            UserCreateOutputObjects     ( );
        virtual void            UserExec                    ( Option_t*         option );
        virtual void            Terminate                   ( Option_t*         option );
    
    // Setters
        void                    fSetMCFlag                  ( Bool_t MCFlag )           { kMCbool = MCFlag; }
        void                    fSetPhiFlag                 ( Bool_t PhiFlag )          { kPhibool = PhiFlag; }
        void                    fSetKaonFlag                ( Bool_t KaonFlag )         { kKaonbool = KaonFlag; }
    
    private:
        void                    fSetZero                    ();
        void                    fPostData                   ();
        bool                    fIsPrimaryVertexCandidate   ( AliAODEvent*      event );
        bool                    fIsTrackCandidate           ( AliAODTrack *     track );
        void                    fSetKaonPID                 ( AliAODTrack *     track );
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
        Bool_t                  kPhibool;           // Phi tree Flag
        Bool_t                  kKaonbool;          // Kaon tree Flag
        
        // Tree
        TTree*                  fKaonCandidate;     //! output tree for Signal
        TTree*                  fPhiCandidate;      //! output tree for Signal
        TTree*                  fKaonEfficiency;    //! output tree for MC Truth
        TTree*                  fPhiEfficiency;     //! output tree for MC Truth

        // Event Variables
        Float_t                 fMultiplicity;      //! Event Multiplicity
        Int_t                   faKaon      [1024];
         
        // Tree Variables ( PhiCandidate )
        Int_t                   fnPhi;              //! Number of Phis produced found
        Float_t                 fInvMass    [1024]; //! Invariant Mass
        Float_t                 fPhiPx      [1024]; //! Phi Px
        Float_t                 fPhiPy      [1024]; //! Phi Py
        Float_t                 fPhiPz      [1024]; //! Phi Pz
        short unsigned Int_t    fiKaon      [1024]; //! iKaon
        short unsigned Int_t    fjKaon      [1024]; //! jKaon
    
        // Tree Variables ( KaonCandidate )
        Int_t                   fnKaon;              //! Number of Phis produced found
        Float_t                 fKaonPx     [1024]; //! Kaon Px
        Float_t                 fKaonPy     [1024]; //! Kaon Py
        Float_t                 fKaonPz     [1024]; //! Kaon Pz
        short signed Int_t      fCharge     [1024]; //! Kaon Charge
        short signed Int_t      fTOFSigma   [1024]; //! PID TOF
        short signed Int_t      fTPCSigma   [1024]; //! PID TPC
         
        // Tree Variables ( PhiEfficiency )
        Int_t                   fnPhiTru;              //! Number of Phis produced found
        Bool_t                  fPbEta      [1024]; //! bEta
        Bool_t                  fPbRec      [1024]; //! bRec
        Bool_t                  fPbKdc      [1024]; //! bKdc
        Float_t                 fPpT        [1024]; //! Transverse Momentum
        
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
