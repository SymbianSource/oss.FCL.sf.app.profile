/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Setting page class for 3D settings.
*
*/



// INCLUDES
#include "CProfile3dSettingPage.h"
#include "ProfileSettingsView.hrh"

#include <CFLDController.h>
#include <profilesettingsview.rsg>
#include <apgcli.h>	// For RApaLsSession
#include <bautils.h> // For BaflUtils
#include <centralrepository.h>
#include <ProfileEngineInternalCRKeys.h>
#include <ProfileInternal.hrh>

// CONSTANTS
namespace
	{
    const TInt K3DEffectBase( 1000 );
    const TInt K3DEchoBase( 2000 );

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
// CProfile3dSettingPage::CProfile3dSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfile3dSettingPage::CProfile3dSettingPage(
    TInt aResourceID,
    MAknQueryValue& aQueryValue,
    const TDesC& aRingingTone,
    const TInt& aRingingType,
    const TBool& aVibratingAlert,
    const TInt& aVolume,
    const TInt& a3DEffectOrEcho,
    TInt aIdentifier )
    :   CAknPopupSettingPage( aResourceID, aQueryValue ),
        iRingingTone( aRingingTone ),
        iRingingType( aRingingType ),
        iVibratingAlert( aVibratingAlert ),
        iVolume( aVolume ),
        i3DEffectOrEcho( a3DEffectOrEcho ),
        iIdentifier( aIdentifier )
    {
    }

// -----------------------------------------------------------------------------
// CProfile3dSettingPage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfile3dSettingPage::ConstructL()
    {
    CAknPopupSettingPage::ConstructL();

	TBool showErrorMsgs( EFalse );
	
    iFs = new ( ELeave ) RFs;
    User::LeaveIfError( iFs->Connect() );

	CheckRingingToneTypeL();

    iController = CFLDController::NewL( showErrorMsgs, KRepeatDelayInMicroSeconds );
	iController->CompleteConstructionL( Window() );
	iController->SetRingingType( iRingingType );
	iController->SetVibra( iVibratingAlert );
	iController->SetVolume( iVolume );
    }

// Destructor.
CProfile3dSettingPage::~CProfile3dSettingPage()
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
// CProfile3dSettingPage::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CProfile3dSettingPage::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if( aType == EEventKey )
        {
        if( ( aKeyEvent.iCode == EKeyUpArrow ) ||
            ( aKeyEvent.iCode == EKeyDownArrow ) )
            {
            // Get current setting
            TInt currentValue = PopupSettingListBox()->CurrentItemIndex();

            // Call base classes method
            TKeyResponse response( CAknPopupSettingPage::OfferKeyEventL( aKeyEvent, aType ) );

            // Get new setting
            TInt newValue = PopupSettingListBox()->CurrentItemIndex();

            if( newValue != currentValue )
                {
                // Setting has changed
                if( ( iIdentifier == EProfileSetting3dEffectId && newValue != EProfile3DEffectOff ) ||
                	( iIdentifier == EProfileSetting3dEchoId && i3DEffectOrEcho != EProfile3DEffectOff ) )
                	{
                    // Set 3D parameters
                	if( iIdentifier == EProfileSetting3dEffectId )
                		{
                        iController->Set3dEffects( K3DEffectBase + newValue );
                        iController->Set3dEffects( K3DEchoBase + i3DEffectOrEcho );
                		}
                	else
                		{
                        iController->Set3dEffects( K3DEffectBase + i3DEffectOrEcho );
                        iController->Set3dEffects( K3DEchoBase + newValue );
                		}
                	iController->Set3dEffects( ETrue );
                	}
                else
                	{
                	iController->Set3dEffects( EFalse );
                	}
                iController->HandleFileListBoxEventL(
                    MFLDFileListBoxObserver::EFocusChanged, iPreviewTone->Des() );
                }
            return response;
            }
        // Cancels playback on any key event
        iController->HandleFileListBoxEventL(
            MFLDFileListBoxObserver::EOtherKeyEvent, iPreviewTone->Des() );
        }

    return CAknPopupSettingPage::OfferKeyEventL( aKeyEvent, aType );
    }


// -----------------------------------------------------------------------------
// CProfile3dSettingPage::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CProfile3dSettingPage::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    if( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        // Get current setting
        iTempValue = PopupSettingListBox()->CurrentItemIndex();
        }
    else if( aPointerEvent.iType == TPointerEvent::EButton1Up )
    	{
	    CAknPopupSettingPage::HandlePointerEventL( aPointerEvent );
        // Get new setting
        TInt newValue = PopupSettingListBox()->CurrentItemIndex();
        if( newValue != iTempValue )
            {
            // Setting has changed
            if( ( iIdentifier == EProfileSetting3dEffectId && newValue != EProfile3DEffectOff ) ||
            	( iIdentifier == EProfileSetting3dEchoId && i3DEffectOrEcho != EProfile3DEffectOff ) )
            	{
                // Set 3D parameters
            	if( iIdentifier == EProfileSetting3dEffectId )
            		{
                    iController->Set3dEffects( K3DEffectBase + newValue );
                    iController->Set3dEffects( K3DEchoBase + i3DEffectOrEcho );
            		}
            	else
            		{
                    iController->Set3dEffects( K3DEffectBase + i3DEffectOrEcho );
                    iController->Set3dEffects( K3DEchoBase + newValue );
            		}
            	iController->Set3dEffects( ETrue );
            	}
            else
            	{
            	iController->Set3dEffects( EFalse );
            	}

            iController->HandleFileListBoxEventL(
                MFLDFileListBoxObserver::EFocusChanged, iPreviewTone->Des() );
            }
        return;
        }
    // Cancels playback on any key event
    iController->HandleFileListBoxEventL(
        MFLDFileListBoxObserver::EOtherKeyEvent, iPreviewTone->Des() );

    CAknPopupSettingPage::HandlePointerEventL( aPointerEvent );
    }


// -----------------------------------------------------------------------------
// CProfile3dSettingPage::CheckRingingToneTypeL
// -----------------------------------------------------------------------------
//
void CProfile3dSettingPage::CheckRingingToneTypeL()
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
