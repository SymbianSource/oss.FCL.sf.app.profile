/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Setting page class for ringing volume setting.
*
*/



// INCLUDES
#include "CProfilePlayingVolumeSettingPage.h"

#include <AknQueryDialog.h>
#include <CFLDController.h>
#include <profilesettingsview.rsg>
#include "CProfileStruct.h" // KProfileMaxProfileNameLength
#include <apgcli.h>	// For RApaLsSession
#include <bautils.h> // For BaflUtils
#include <centralrepository.h>
#include <ProfileEngineInternalCRKeys.h>

// CONSTANTS
namespace
	{
	const TInt KRepeatDelayInMicroSeconds( 1000000 ); // One second

#ifdef RD_VIDEO_AS_RINGING_TONE
	_LIT( KFLDCommonVideoType, "video/*" );
	_LIT( KFLDRM1VideoMimeType, "application/vnd.rn-realmedia" );
	_LIT( KFLDRM2VideoMimeType, "application/x-pn-realmedia" );
	_LIT( KFLDSDPVideoMimeType, "application/sdp" );
#endif

	}
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::CProfilePlayingVolumeSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfilePlayingVolumeSettingPage::CProfilePlayingVolumeSettingPage(
    TInt aResourceID,
    TInt& aVolume,
    const TDesC& aRingingTone,
    const TInt& aRingingType,
    const TBool& aVibratingAlert,
    TBool aDisplayQuery )
    :   CAknVolumeSettingPage( aResourceID, aVolume ),
        iRingingTone( aRingingTone ),
        iRingingType( aRingingType ),
        iVibratingAlert( aVibratingAlert ),
        iDisplayQuery( aDisplayQuery )
    {
    }

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilePlayingVolumeSettingPage::ConstructL()
    {
    CAknVolumeSettingPage::ConstructL();
	TBool showErrorMsgs( EFalse );
	
    iFs = new ( ELeave ) RFs;
    User::LeaveIfError( iFs->Connect() );

	CheckRingingToneTypeL();

    iController = CFLDController::NewL( showErrorMsgs, KRepeatDelayInMicroSeconds );
	iController->CompleteConstructionL( Window() );
	iController->SetRingingType( iRingingType );
	iController->SetVibra( iVibratingAlert );
    }

// Destructor.
CProfilePlayingVolumeSettingPage::~CProfilePlayingVolumeSettingPage()
    {
    if( iFs )
    	{
    	iFs->Close();
    	}
    delete iFs;
    delete iController;
    delete iPreviewTone;
    }

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CProfilePlayingVolumeSettingPage::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if( aType == EEventKey )
        {
        if( ( aKeyEvent.iCode == EKeyLeftArrow ) ||
            ( aKeyEvent.iCode == EKeyRightArrow ) )
            {
            // Cache pointer
            CAknVolumeControl* volumeControl = VolumeControl();
            // Get current volume level
            TInt currentVolume( volumeControl->Value() );
            // Call base classes method
            TKeyResponse response( CAknVolumeSettingPage::OfferKeyEventL( aKeyEvent, aType ) );
            // Get new volume level
            TInt newVolume( volumeControl->Value() );
            if( newVolume != currentVolume )
                {
                // Volume level has changed
                CheckRingingToneTypeL();
                iController->SetVolume( newVolume );
                iController->HandleFileListBoxEventL(
                    MFLDFileListBoxObserver::EFocusChanged, iPreviewTone->Des() );
                }
            return response;
            }
        // Cancels playback on any key event
        iController->HandleFileListBoxEventL(
            MFLDFileListBoxObserver::EOtherKeyEvent, iPreviewTone->Des() );
        }
    return CAknVolumeSettingPage::OfferKeyEventL( aKeyEvent, aType );
    }


// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CProfilePlayingVolumeSettingPage::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    if( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        // Get current volume level
        iTempVolume = VolumeControl()->Value();
        }
    else if( aPointerEvent.iType == TPointerEvent::EButton1Up )
    	{
	    CAknVolumeSettingPage::HandlePointerEventL( aPointerEvent );
        // Get new volume level
        TInt newVolume( VolumeControl()->Value() );
        if( newVolume != iTempVolume )
            {
            // Volume level has changed
            CheckRingingToneTypeL();
            iController->SetVolume( newVolume );
            iController->HandleFileListBoxEventL(
                MFLDFileListBoxObserver::EFocusChanged, iPreviewTone->Des() );
            }
        return;
        }
    // Cancels playback on any key event
    iController->HandleFileListBoxEventL(
        MFLDFileListBoxObserver::EOtherKeyEvent, iPreviewTone->Des() );

    CAknVolumeSettingPage::HandlePointerEventL( aPointerEvent );
    }


// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::OkToExitL
// -----------------------------------------------------------------------------
//
TBool CProfilePlayingVolumeSettingPage::OkToExitL(TBool aAccept)
    {
    if( aAccept )
        {
        CAknVolumeControl* volumeControl = VolumeControl();
        if(( volumeControl->Value() == KProfileMaxVolumeLevel ) // Maximum volume level
        && (iDisplayQuery)) // Maximum volume query is wanted
            {
            iController->HandleFileListBoxEventL(
                MFLDFileListBoxObserver::EListBoxClosed, KNullDesC );

            CAknQueryDialog* dlg = CAknQueryDialog::NewL();
            TBool returnValue( dlg->ExecuteLD( R_PROFILE_VOLUME_CONFIRMATION_QUERY ) );
            if( !returnValue )
                {
                // User didn't want to set volume to maximum level, set it one down.
                volumeControl->SetValue( KProfileMaxVolumeLevel - 1 );
                volumeControl->DrawDeferred();
                }
            return returnValue;
            }
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingPage::CheckRingingToneTypeL
// -----------------------------------------------------------------------------
//
void CProfilePlayingVolumeSettingPage::CheckRingingToneTypeL()
	{
	// Get default tone from cenrep
	TFileName defaultTone;
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
	CleanupStack::PushL( cenrep );
	User::LeaveIfError( cenrep->Get( KProEngDefaultRingingTone, defaultTone ) );
	CleanupStack::PopAndDestroy( cenrep );

	if( !BaflUtils::FileExists( *iFs, iRingingTone ) )
		{
		// If the file does not exist, use default tone for volume preview
		iPreviewTone = defaultTone.AllocL();
		return;
		}

#ifdef RD_VIDEO_AS_RINGING_TONE
	// Find out MIME type
	RApaLsSession apaLsSession;
	User::LeaveIfError( apaLsSession.Connect() );
	CleanupClosePushL( apaLsSession );
	TUid dummyUid = { 0 }; // instantiate as zero
	TDataType dataType( dummyUid );
	User::LeaveIfError(
		apaLsSession.AppForDocument( iRingingTone, dummyUid, dataType ) );
	CleanupStack::PopAndDestroy(); // apaLsSession.Close()

	// See if this is a video MIME type
	if ( dataType.Des().MatchF( KFLDCommonVideoType ) == 0 ||
		dataType.Des().CompareF( KFLDRM1VideoMimeType ) == 0 ||
		dataType.Des().CompareF( KFLDRM2VideoMimeType ) == 0 ||
		dataType.Des().CompareF( KFLDSDPVideoMimeType ) == 0 )
		{
		iPreviewTone = defaultTone.AllocL();
		}
	else
		{
		iPreviewTone = iRingingTone.AllocL();
		}
#else
	iPreviewTone = iRingingTone.AllocL();
#endif
	}

// End of File
