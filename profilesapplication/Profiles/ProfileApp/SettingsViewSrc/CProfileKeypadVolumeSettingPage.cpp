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
* Description:  Setting page class for keypad volume setting.
*
*/



// INCLUDES
#include "CProfileKeypadVolumeSettingPage.h"

#include <AudioPreference.h>
#include <aknsoundsystem.h>
#include <StringLoader.h>	// For StringLoader
#include <aknnotewrappers.h> // For CAknInformationNote
#include <profilesettingsview.rsg>

// for power save mode handling
#include <psmsettings.h>
#include <psmsrvdomaincrkeys.h>
#include <centralrepository.h>

namespace
	{
// CONSTANTS
#ifdef __WINS__
	// These constants are for WINS only because sequences are not played in emulator.
	const TInt KKeypadToneFrequency( 1760 ); /// In Hz
	const TInt KKeypadToneDuration( 20000 ); /// In microseconds
#endif // __WINS__
	}
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::CProfileKeypadVolumeSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileKeypadVolumeSettingPage::CProfileKeypadVolumeSettingPage(
	TInt aResourceID,
	MAknQueryValue& aQueryValue,
	CAknKeySoundSystem* aSoundSystem )
	:	CAknPopupSettingPage( aResourceID, aQueryValue ),
		iSoundSystem( aSoundSystem )
	{
	}

// Destructor.
CProfileKeypadVolumeSettingPage::~CProfileKeypadVolumeSettingPage()
	{
	if( iAudioUtility )
		{
		if( iAudioUtility->State() != EMdaAudioToneUtilityNotReady )
			{
			// Set volume back to original
			iAudioUtility->SetVolume( iOriginalVolume );
			}
		delete iAudioUtility;
		}
	if( iSoundSystem )
		{
		// Re-enable keypad sounds
		iSoundSystem->PopContext();
		}
	}

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileKeypadVolumeSettingPage::ConstructL()
	{
	User::LeaveIfNull( iSoundSystem );

	CAknPopupSettingPage::ConstructL();

	// Own construction starts here

	// Disable keypad sounds
	iSoundSystem->PushContextL( R_AVKON_SILENT_SKEY_LIST );

	// Create audio player and prepare to play a tone
	iAudioUtility = CMdaAudioToneUtility::NewL( *this );

#ifdef __WINS__ // Sequences are not supported on emulator
	iAudioUtility->PrepareToPlayTone( KKeypadToneFrequency,
		TTimeIntervalMicroSeconds( KKeypadToneDuration ) );
#else
	CAknSoundInfo* soundInfo = CAknSoundInfo::NewL();
	CleanupStack::PushL( soundInfo );
	User::LeaveIfError( iSoundSystem->RequestSoundInfoL(
		EAvkonSIDStandardKeyClick, *soundInfo ) );
	iAudioUtility->PrepareToPlayDesSequence( *soundInfo->iSequence );
	CleanupStack::PopAndDestroy( soundInfo );
#endif // __WINS__
	}


// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::OkToExitL
// -----------------------------------------------------------------------------
//
TBool CProfileKeypadVolumeSettingPage::OkToExitL( TBool aAccept )
    {
    if( aAccept )
        {
        TInt psm = EPsmsrvModeNormal;
        CRepository* cenrep = CRepository::NewLC( KCRUidPowerSaveMode );
        cenrep->Get( KPsmCurrentMode, psm );
        CleanupStack::PopAndDestroy( cenrep );
        if( psm == EPsmsrvModePowerSave )
            {
            HBufC* buf = StringLoader::LoadLC( R_PROFILE_TEXT_PSM_INFONOTE );
            CAknInformationNote* queryDialog = new (ELeave) CAknInformationNote( EFalse );
         	queryDialog->ExecuteLD( *buf );
            CleanupStack::PopAndDestroy( buf );
            }
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CProfileKeypadVolumeSettingPage::OfferKeyEventL(
	const TKeyEvent& aKeyEvent, TEventCode aType )
	{
	// Cache pointer used multiple times
	CAknSetStyleListBox* listBoxControl = ListBoxControl();
	// Get current index from listbox
	TInt oldIndex( listBoxControl->CurrentItemIndex() );
	// Call OfferKeyEventL of base class
	TKeyResponse response( CAknPopupSettingPage::OfferKeyEventL( aKeyEvent, aType ) );
	// Get audio player state
	TInt state( iAudioUtility->State() );
	// Get new index from listbox
	TInt newIndex( listBoxControl->CurrentItemIndex() );

	if( ( oldIndex != newIndex ) && // check if listbox focus has changed
		( newIndex != 0 ) && // check that new focus is not on first item "Off"
		( state != EMdaAudioToneUtilityNotReady ) ) // check that tone player is ready
		{
		// Resolve last index of listbox
		TInt lastIndex( listBoxControl->Model()->MatchableTextArray()->MdcaCount() - 1 );
		// Resolve correct volume from current listbox index
		TInt volume( iAudioUtility->MaxVolume() * newIndex / lastIndex );

		if( state == EMdaAudioToneUtilityPlaying )
			{
			// Cancel play if still playing previous keyclick
	        iAudioUtility->CancelPlay();
			}
		iAudioUtility->SetVolume( volume );
		iAudioUtility->Play();
		}
	return response;
	}

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::MatoPrepareComplete
// -----------------------------------------------------------------------------
//
void CProfileKeypadVolumeSettingPage::MatoPrepareComplete( TInt aError )
	{
	if( !aError )
		{
		// After prepare is completed, store original volume level
		iOriginalVolume = iAudioUtility->Volume();
		// Set priority
		iAudioUtility->SetPriority( KAudioPriorityKeyPress,
			TMdaPriorityPreference( KAudioPrefKeyPressPreview ) );
		}
	}

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingPage::MatoPlayComplete
// Error ignored because if playing of the keypad tone is unsuccesful
// nothing can be done.
// -----------------------------------------------------------------------------
//
void CProfileKeypadVolumeSettingPage::MatoPlayComplete( TInt /*aError*/ )
	{
	}

// End of File
