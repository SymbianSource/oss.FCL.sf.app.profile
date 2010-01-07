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
* Description:  Container class of settings view.
*
*/



// CLASS HEADER
#include    "CProfileSettingsContainer.h"

// INTERNAL INCLUDES
#include    "CProfileEngineHandler.h"
#include    "CProfileIndexHandler.h"
#include    "CProfileTabHandler.h"
#include    "ProfileSettingsView.hrh"
#include    "CProfile3dSettingItem.h"
#include    "CProfileFeedbackSettingItem.h"
#include    "CProfileVibraSettingItem.h"
#include    "CProfilePlayingVolumeSettingItem.h"
#include    "CProfileNameSettingItem.h"
#include    "CProfileKeypadVolumeSettingItem.h"
#include    "CProfileFileListSettingItem.h"
#include    "CProfileAlertForSettingItem.h"
#include    "CProfileStruct.h"
#include    "mprofileslocalfeatures.h"
#include    "profilesvariant.hrh"  // KProEngFeatureIdVTRingingTone
#include	"CProfileSettingsView.h"
#include	"MProfileViewHandler.h" 
#include    "ProfilesDebug.h"

// EXTERNAL INCLUDES
#include 	<AknDef.h>
#include    <aknview.h>
#include    <akntitle.h>
#include    <akntextsettingpage.h>
#include    <aknsoundsystem.h>
#include    <eikapp.h>
#include    <csxhelp/mode.hlp.hrh>
#include    <featmgr.h>
#include    <mprofileextended.h>

#include    <mprofile3dtonesettings.h>
#include    <mprofilefeedbacksettings.h>
#include    <mprofileextrasettings.h>

#include    <mprofileengineextended.h>
#include    <mprofilesnamesarray.h>
#include    <mprofilename.h>
#include    <cprofilechangenotifyhandler.h>
#include    <ProfileSettingsView.rsg>
#include 	<data_caging_path_literals.hrh>
#include 	<centralrepository.h>
#include    <settingsinternalcrkeys.h>
#include	<bautils.h>	// For BaflUtils
#include	<StringLoader.h>	// For StringLoader
#include	<profileenginesdkcrkeys.h>	// For KCRUidProfileEngine
#include	<profileengineinternalcrkeys.h>	// For KProEngDefaultRingingTone
#include	<AknQueryDialog.h>	// For CAknQueryDialog
#include    "CProfileNaviPaneContainer.h" // For CProfileNaviPaneContainer
#include    <aknnotewrappers.h> // For CAknInformationNote

// for power save mode handling
#include    <psmsettings.h>
#include    <psmsrvdomaincrkeys.h>
#include    <e32property.h>
#include    <profileengineprivatepskeys.h>

namespace
	{
	// CONSTANTS
	const TUid KUidProfileApp = { 0x100058F8 }; // UID of the application
	const TInt KVoIPOFF = 0;

	const TInt KFileCheckDelay = 100000; // microseconds
	const TInt KFileCheckInterval = 5000000;

	_LIT( KSettingsViewResourceFileName, "z:PROFILESETTINGSVIEW.RSC" );

	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::CProfileSettingsContainer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileSettingsContainer::CProfileSettingsContainer(
    MProfileViewHandler& aProfileViewHandler,
    CProfileEngineHandler& aEngineHandler,
    MProfileExternalSettingsHandler& aExternalSettingsHandler,
    CProfileIndexHandler* aIndexHandler,
    CProfileTabHandler* aTabHandler,
    CAknTitlePane* aTitlePane )
    :
    iResourceLoader( *iCoeEnv ),
    iProfileViewHandler( aProfileViewHandler ),
    iEngineHandler( aEngineHandler ),
    iExternalSettingsHandler( aExternalSettingsHandler ),
    iIndexHandler( aIndexHandler ),
    iTabHandler( aTabHandler ),
    iTitlePane( aTitlePane ),
    iAlternateLineService( EFalse )
    {
    SetMopParent( &( iProfileViewHandler.View() ) );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::ConstructL( const TRect& aRect,
                                            TInt aResourceId )
    {
    iFs = new ( ELeave ) RFs;
    User::LeaveIfError( iFs->Connect() );

    PRODEBUG1( "CProfileSettingsContainer ConstructL(%d)",aResourceId );
    if( iIndexHandler )
        {
        iIndexHandler->CreateIndexArrayL();
        }

    iCopyProfile = CProfileStruct::NewL();

    iLocalFeatures = &( iEngineHandler.Engine()->LocalFeatures() );

    // Read settings of currently selected profile
    ReadProfileL();

    // Read alternate line service setting and register as observer
    if( iSSSettings.Open() == KErrNone )
        {
        iSSSettings.Register( ESSSettingsAls, *this ); // ignore return value
        TInt als( ESSSettingsAlsNotSupported );
        iSSSettings.Get( ESSSettingsAls, als ); // ignore return value
        if( als != ESSSettingsAlsNotSupported )
            {
            iAlternateLineService = ETrue;
            }
        }

	// CRepository session for TTS ringingtone
	iTTSSession = CRepository::NewL( KCRUidProfilesLV );
	
	iDefaultTone = new( ELeave ) TFileName();
	
    iSession = CRepository::NewL(KCRUidProfileEngine);
    User::LeaveIfError( iSession->Get( KProEngDefaultRingingTone, *iDefaultTone ) );

	TInt VoIPSupported( 0 );
	iVoIPVariation = CRepository::NewL( KCRUidTelephonySettings );
	iVoIPVariation->Get( KDynamicVoIP, VoIPSupported );
	iVoIPVariationNotifier = CCenRepNotifyHandler::NewL( *this, *iVoIPVariation,
        CCenRepNotifyHandler::EIntKey, KDynamicVoIP );
    iVoIPVariationNotifier->StartListeningL();
  
    iVoIPStatus = KVoIPOFF != VoIPSupported;

    iKeySounds = static_cast< CAknAppUi* > ( iEikonEnv->AppUi() )->KeySounds();

	TParse* fp = new(ELeave) TParse();
	fp->Set(KSettingsViewResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL);
	TFileName resourceFileName( fp->FullName() );
	delete fp;

	iResourceLoader.OpenL( resourceFileName );

    // Construct the setting items
    CAknSettingItemList::ConstructFromResourceL( aResourceId );
    // Hide all items that must be hidden
    SetHiddenFlagsL();

    if( iIndexHandler )
        {
        iIndexHandler->SetListBox( ListBox() );
        }
    if( iTabHandler )
        {
        iTabHandler->SetTabObserver( this );
        }
    else
        {
        // If the iTabHandler is NULL, set the navi label text to 
        // the name of current profile.
        iNaviPaneContainer = CProfileNaviPaneContainer::NewL(
                *( iAvkonAppUi->StatusPane() ), iEngineHandler );

        MProfilesNamesArray* nameArray = iEngineHandler.IdArray();
        iNaviPaneContainer->SetNaviPaneTextL( 
            nameArray->MdcaPoint( iIndexHandler->CurrentProfileIndex() ) );
        }

    iNotifier = CProfileChangeNotifyHandler::NewL( this );

    iExternalSettingsHandler.SetObserver( *this );
    SetRect( aRect );
    ActivateL();
    PRODEBUG1( "CProfileSettingsContainer ConstructL(%d) return",aResourceId );

    //CheckFileExistenceL();
    // If CheckFileExistenceL is called directly from
    // ConstructL, DoActivateL in ProfileApp::CProfileSettingsView will be
    // blocked while the notes are displayed, causing problems.
	// This timer will trigger only once (after 100 ms). After
	// that it will be destroyed.
	iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
	iPeriodic->Start(KFileCheckDelay,
					KFileCheckInterval,
					TCallBack( CProfileSettingsContainer::StartFileCheckL, this )
					);

    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::NewL
// Two-phased constructor. Note, this constructor is DEPRECATED and will return
// only NULL if called.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileSettingsContainer* CProfileSettingsContainer::NewL(
    CAknView& /* aAknView */,
    CProfileEngineHandler& /* aEngineHandler */,
    CProfileIndexHandler* /* aIndexHandler */,
    CProfileTabHandler* /* aTabHandler */,
    CAknTitlePane* /* aTitlePane */ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileSettingsContainer* CProfileSettingsContainer::NewL(
    MProfileViewHandler& aProfileViewHandler,
    CProfileEngineHandler& aEngineHandler,
    MProfileExternalSettingsHandler& aExternalSettingsHandler,
    CProfileIndexHandler* aIndexHandler,
    CProfileTabHandler* aTabHandler,
    CAknTitlePane* aTitlePane )
    {
    return CProfileSettingsContainer::NewL( aProfileViewHandler, aEngineHandler,
            aExternalSettingsHandler, aIndexHandler, aTabHandler, aTitlePane,
            R_PROFILE_SETTING_ITEM_LIST );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileSettingsContainer* CProfileSettingsContainer::NewL(
    MProfileViewHandler& aProfileViewHandler,
    CProfileEngineHandler& aEngineHandler,
    MProfileExternalSettingsHandler& aExternalSettingsHandler,
    CProfileIndexHandler* aIndexHandler,
    CProfileTabHandler* aTabHandler,
    CAknTitlePane* aTitlePane,
    TInt aResourceId )
    {
    CProfileSettingsContainer* self = new( ELeave ) CProfileSettingsContainer(
        aProfileViewHandler, aEngineHandler, aExternalSettingsHandler,
        aIndexHandler, aTabHandler, aTitlePane );
    CleanupStack::PushL( self );
    self->ConstructL( aProfileViewHandler.View().ClientRect(), aResourceId );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor.
CProfileSettingsContainer::~CProfileSettingsContainer()
    {
	if( iPeriodic )
		{
		iPeriodic->Cancel();
		delete iPeriodic;
		}

    if( iFs )
    	{
    	iFs->Close();
    	}
    delete iFs;

    PRODEBUG( "~CProfileSettingsContainer begin" );
    delete iNotifier;
    PRODEBUG( "~CProfileSettingsContainer 2" );
    if( iTabHandler )
        {
        iTabHandler->SetTabObserver( NULL );
        }
    PRODEBUG( "~CProfileSettingsContainer 3" );
    if( iIndexHandler )
        {
        iIndexHandler->SetListBox( NULL );
        }
    PRODEBUG( "~CProfileSettingsContainer 4" );
    iResourceLoader.Close();
    PRODEBUG( "~CProfileSettingsContainer 5" );
    ReleaseProfile();
    PRODEBUG( "~CProfileSettingsContainer 6" );
    delete iCopyProfile;
    PRODEBUG( "~CProfileSettingsContainer 7" );

    if ( iVoIPVariationNotifier )
        {
        iVoIPVariationNotifier->StopListening();
        delete iVoIPVariationNotifier;
        }
    if ( iVoIPVariation )
        {
    	delete iVoIPVariation;
    	}
    	  
    delete iTTSSession;
    delete iSession;
    
    delete iDefaultTone;
    PRODEBUG( "~CProfileSettingsContainer 8" );
    
   	iSSSettings.Close();
    PRODEBUG( "~CProfileSettingsContainer end" );
    delete iNaviPaneContainer;
    }

// ------------------------------------------------------------------------------
// CProfileSettingsContainer::StartFileCheckL
// ------------------------------------------------------------------------------
//
TInt CProfileSettingsContainer::StartFileCheckL( TAny* aObj )
	{
	static_cast<CProfileSettingsContainer*>(aObj)->DoFileCheckL();
	return 1;
	}

// ------------------------------------------------------------------------------
// CProfileSettingsContainer::DoFileCheckL
// ------------------------------------------------------------------------------
//
void CProfileSettingsContainer::DoFileCheckL()
	{
	if( iPeriodic )
		{
		iPeriodic->Cancel();
		delete iPeriodic;
		iPeriodic = NULL;
		}

	CheckFileExistenceL();
	}

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::AllowedToChange
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CProfileSettingsContainer::AllowedToChange(
    TInt aSettingItemId ) const
    {
    TUint32 flags( iProfile->ModifiableFlags() );
    switch( aSettingItemId )
        {
        // Fall through: If ringing tone is not allowed to change, then none of
        // these setting items are allowed to change.
        case EProfileSettingRingingToneId:
        case EProfileSettingRingingToneLine1Id:
        case EProfileSettingRingingToneLine2Id:
            {
            TInt profileId( 0 );
            // Get id of the edited profile
            profileId=iProfile->ProfileName().Id();
            if( profileId == EProfileOffLineId )
                {
                // VOIP Changes: WLAN can be enabled also in off-line profile
                TBool featuresSupported(
                        FeatureManager::FeatureSupported(KFeatureIdCommonVoip)
                        &&
                        FeatureManager::FeatureSupported(KFeatureIdProtocolWlan)
                        );
                return ( ( flags & EProfileFlagRingingTone ) &&
                         featuresSupported && iVoIPStatus );
                }
            else
                {
                return flags & EProfileFlagRingingTone;
                }
            }


        case EProfileSettingVideoCallToneId:
            {
            return flags & EProfileFlagVideoCallRingingTone;
            }
	    case EProfileSettingRingingTypeId:
            {
            return flags & EProfileFlagRingingType;
            }
        case EProfileSettingRingingVolumeId:
            {
            return flags & EProfileFlagRingingVolume;
            }
		 case EProfileSettingMediaVolumeId:
            {
            return flags & EProfileFlagMediaVolume;
            }
        case EProfileSettingMessageAlertToneId:
            {
            return flags & EProfileFlagMessageAlertTone;
            }
        case EProfileSettingEmailAlertToneId:
            {
            return flags & EProfileFlagEmailAlertTone;
            }
        case EProfileSettingVibratingAlertId:
            {
            return flags & EProfileFlagVibratingAlert;
            }
        case EProfileSettingEmailVibratingAlertId:
            {
            return flags & EProfileFlagVibratingAlert;
            }           
        case EProfileSettingKeypadVolumeId:
            {
            return flags & EProfileFlagKeypadVolume;
            }
        case EProfileSettingWarningAndGameTonesId:
            {
            return flags & EProfileFlagWarningTones;
            }
		case EProfileSettingTTSRingingToneId:
			{
			return flags & EProfileFlagTextToSpeechTone;
			}
        case EProfileSettingAlertForId:
            {
            return flags & EProfileFlagAlertFor;
            }
        case EProfileSettingProfileNameId:
            {
            return flags & EProfileFlagProfileName;
            }
        case EProfileSettingPTTRingingToneId:
            {
            return flags & EProfileFlagPttTone;
            }
        case EProfileSettingPTTStatusId:
            {
            return flags & EProfileFlagPttStatus;
            }
        case EProfileSettingTactileFeedbackId:
            {
            return flags & EProfileFlagTactileFeedback;
            }
    	case EProfileSettingAudioFeedbackId:
            {
            return flags & EProfileFlagAudioFeedback;
            }            
        case EProfileSetting3dEffectId:
            {
            return flags & EProfileFlag3DEffect;
            }
 		case EProfileSetting3dEchoId:
            {
            return flags & EProfileFlag3DEcho;
            }            
             
        default:
            {
            return iExternalSettingsHandler.AllowedToChange( aSettingItemId );
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::ReleaseProfile
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::ReleaseProfile()
    {
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::ReadProfileL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::ReadProfileL()
    {
    TInt profileId( KErrNotFound );
    if( iIndexHandler )
        {
        // Index handler is set, get id of the edited profile
        profileId = iEngineHandler.IdForIndex(
            iIndexHandler->CurrentProfileIndex() );
        }
    else
        {
        // Index handler is not set.
        profileId = User::LeaveIfError(
            iEngineHandler.Engine()->ActiveProfileId() );
        }
    MProfileExtended* profile = iEngineHandler.Engine()->ProfileLC( profileId );
    iCopyProfile->LoadL( profile );
    CleanupStack::Pop(); // profile
    ReleaseProfile(); // delete old iProfile
    iProfile = profile;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::SetHiddenFlagsL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::SetHiddenFlagsL()
    {
    CAknSettingItemArray* array = SettingItemArray();
    TUint32 flags( iProfile->VisibleFlags() );
    TInt profileId( 0 );

    TBool hide3DEcho = EFalse;
    TInt effect = iProfile->ProfileExtraSettings().Profile3DToneSettings().Effect();
    if ( effect == EProfile3DEffectOff || effect == EProfile3DEffectStereoWidening )
        {
        hide3DEcho = ETrue;
        }

    // Get id of the edited profile
    profileId=iProfile->ProfileName().Id();

    if( profileId == EProfileOffLineId )
        {
        TBool featuresSupported(
                FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) &&
                FeatureManager::FeatureSupported( KFeatureIdProtocolWlan) );
        SetItemHidden( EProfileSettingRingingToneId, array,
            ( iAlternateLineService ) || ( ( ~flags & EProfileFlagRingingTone )
            || !featuresSupported || !iVoIPStatus ) );
        SetItemHidden( EProfileSettingRingingToneLine1Id, array,
            ( !iAlternateLineService ) || ( ( ~flags & EProfileFlagRingingTone )
            || !featuresSupported || !iVoIPStatus ) );
        SetItemHidden( EProfileSettingRingingToneLine2Id, array,
            ( !iAlternateLineService ) || ( ( ~flags & EProfileFlagRingingTone )
            || !featuresSupported || !iVoIPStatus ) );
	  	SetItemHidden( EProfileSetting3dEffectId, array,
	  		!( ( FeatureManager::FeatureSupported( KFeatureId3DRingingTones ) ) &&
	        ( flags & EProfileFlag3DEffect ) && featuresSupported ) );
		SetItemHidden( EProfileSetting3dEchoId, array,
	  		!( ( FeatureManager::FeatureSupported( KFeatureId3DRingingTones ) ) &&
	        ( flags & EProfileFlag3DEcho ) && featuresSupported ) );
        }
    else
        {
        SetItemHidden( EProfileSettingRingingToneId, array,
            ( iAlternateLineService ) || ( ~flags & EProfileFlagRingingTone ) );
        SetItemHidden( EProfileSettingRingingToneLine1Id, array,
            ( !iAlternateLineService ) || ( ~flags & EProfileFlagRingingTone ) );
        SetItemHidden( EProfileSettingRingingToneLine2Id, array,
            ( !iAlternateLineService ) || ( ~flags & EProfileFlagRingingTone ) );
	  	SetItemHidden( EProfileSetting3dEffectId, array,
	  		!( ( FeatureManager::FeatureSupported( KFeatureId3DRingingTones ) ) &&
	        ( flags & EProfileFlag3DEffect ) ) );
		SetItemHidden( EProfileSetting3dEchoId, array,
	  		!( ( FeatureManager::FeatureSupported( KFeatureId3DRingingTones ) ) &&
	        ( flags & EProfileFlag3DEcho ) ) );
        }

    if ( hide3DEcho )
        {
      	SetItemHidden( EProfileSetting3dEchoId, array, ETrue );
        }

    SetItemHidden( EProfileSettingVideoCallToneId, array,
        !( ( iLocalFeatures->IsFeatureSupported(
        KProEngFeatureIdVTRingingTone ) ) &&
        ( flags & EProfileFlagVideoCallRingingTone ) &&
        FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) ) );

	SetItemHidden( EProfileSettingMessageAlertToneId, array,
        ~flags & EProfileFlagMessageAlertTone );
        
    SetItemHidden( EProfileSettingEmailAlertToneId, array,
        !( ( FeatureManager::FeatureSupported( KFeatureIdAlwaysOnLineEmail ) ) &&
        ( flags & EProfileFlagEmailAlertTone ) ) );
            
	SetItemHidden( EProfileSettingRingingTypeId, array,
        ~flags & EProfileFlagRingingType );
        
    SetItemHidden( EProfileSettingRingingVolumeId, array,
        ~flags & EProfileFlagRingingVolume );
        
    SetItemHidden( EProfileSettingMediaVolumeId, array,
		!( ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) ) &&
		( flags & EProfileFlagMediaVolume ) ) );
		
	SetItemHidden( EProfileSettingVibratingAlertId, array,
        ~flags & EProfileFlagVibratingAlert );
        
	SetItemHidden( EProfileSettingEmailVibratingAlertId, array,
        ~flags & EProfileFlagVibratingAlert );
    SetItemHidden( EProfileSettingKeypadVolumeId, array,
        ~flags & EProfileFlagKeypadVolume );
        
    SetItemHidden( EProfileSettingWarningAndGameTonesId, array,
        ~flags & EProfileFlagWarningTones );

	// Check if TTS ringingtone is supported
	TBool textToSpeech( EFalse );
   	User::LeaveIfError( iTTSSession->Get( KProfilesLVFlags, textToSpeech ) );
   	textToSpeech &= KProEngFeatureIdTTSRingingTone;

	SetItemHidden( EProfileSettingTTSRingingToneId, array,
		!( textToSpeech && ( flags & EProfileFlagTextToSpeechTone ) ) );

    SetItemHidden( EProfileSettingAlertForId, array,
        ~flags & EProfileFlagAlertFor );

    SetItemHidden( EProfileSettingProfileNameId, array,
        ~flags & EProfileFlagProfileName );

	SetItemHidden( EProfileSettingPTTRingingToneId, array,
		!( ( FeatureManager::FeatureSupported( KFeatureIdOmaPoc ) ) &&
		( flags & EProfileFlagPttTone ) ) );

	SetItemHidden( EProfileSettingPTTStatusId, array,
		!( ( FeatureManager::FeatureSupported( KFeatureIdOmaPoc ) ) &&
		( flags & EProfileFlagPttStatus ) ) );
		
  	SetItemHidden( EProfileSettingTactileFeedbackId, array,
  		!( ( FeatureManager::FeatureSupported( KFeatureIdTactileFeedback ) ) &&
        ( flags & EProfileFlagTactileFeedback ) ) );
	SetItemHidden( EProfileSettingAudioFeedbackId, array,
		!( ( FeatureManager::FeatureSupported( KFeatureIdTactileFeedback ) ) &&
        ( flags & EProfileFlagAudioFeedback ) ) );


	// Check if SilentMode is supported
	TBool silentMode(
	 iLocalFeatures->IsFeatureSupported( KProEngFeatureIdForceSilentMode ) );	
	
	if( ( silentMode ) && ( profileId == EProfileSilentId ) )
		{
		SetItemHidden( EProfileSettingRingingToneId, array, ETrue );
		SetItemHidden( EProfileSettingRingingToneLine1Id, array, ETrue );
		SetItemHidden( EProfileSettingRingingToneLine2Id, array, ETrue );
		SetItemHidden( EProfileSettingVideoCallToneId, array, ETrue );
		SetItemHidden( EProfileSettingTTSRingingToneId, array, ETrue );
		SetItemHidden( EProfileSettingRingingTypeId, array, ETrue );
		SetItemHidden( EProfileSettingRingingVolumeId, array, ETrue );
		SetItemHidden( EProfileSettingMessageAlertToneId, array, ETrue );
		SetItemHidden( EProfileSettingEmailAlertToneId, array, ETrue );
		SetItemHidden( EProfileSettingKeypadVolumeId, array, ETrue );
		SetItemHidden( EProfileSettingWarningAndGameTonesId, array, ETrue );
		SetItemHidden( EProfileSettingPTTRingingToneId, array, ETrue );
		SetItemHidden( EProfileSettingProfileNameId, array, ETrue );
		SetItemHidden( EProfileSettingAudioFeedbackId, array, ETrue );
		SetItemHidden( EProfileSetting3dEffectId, array, ETrue );
		SetItemHidden( EProfileSetting3dEchoId, array, ETrue );
		}

    iExternalSettingsHandler.SetHiddenItems( *array );

    if( iIndexHandler )
    	{
	    iIndexHandler->LoadIndices();
    	}

    HandleChangeInItemArrayOrVisibilityL();
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::UpdateSettingsL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::UpdateSettingsL( TBool aReadProfileSettings )
    {
	// Set listbox invicible to avoid flicker
    CEikListBox* listBox = ListBox();
    listBox->MakeVisible( EFalse );

    // Leavable function calls must be trapped in order to set listbox
    // back to visible again if a leave occurs
    TRAPD( error,
        if( aReadProfileSettings )
            {
            ReadProfileL();
            }
        LoadSettingsL(); // from CAknSettingItemList
        SetHiddenFlagsL();
        );

    if( aReadProfileSettings && iIndexHandler )
        {
        // Set focus back on correct index
        iIndexHandler->LoadIndices();
        }

    // Set listbox back to visible
    listBox->MakeVisible( ETrue );

    // Now proceed with the leave if one occured
    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::EditItemL
// -----------------------------------------------------------------------------
//
EXPORT_C void CProfileSettingsContainer::EditItemL( TInt aIndex, TBool aCalledFromMenu )
    {
    PRODEBUG1( "CProfileSettingsContainer EditItemL(%d) begin",aIndex );

    // Prevent opening of several setting items
    if( iItemEdited )
    	{
    	return;
    	}

    CAknSettingItem* item = SettingItemArray()->At( aIndex );
    TInt identifier( item->Identifier() );

    if ( PowerSaveMode( identifier ) )
        {
        HBufC* buf = StringLoader::LoadLC( R_PROFILE_TEXT_PSM_INFONOTE );
        CAknInformationNote* queryDialog = new (ELeave) CAknInformationNote( ETrue );
     	queryDialog->ExecuteLD( *buf );
        CleanupStack::PopAndDestroy( buf );
        return;
        }
    
    if( !AllowedToChange( identifier ) )
        {
        // If a setting is not allowed to change, "swallow" list box event.
        return;
        }

    HBufC* titleText = NULL;

    if( ( identifier != EProfileSettingRingingToneId ) &&
        ( identifier != EProfileSettingRingingToneLine1Id ) &&
        ( identifier != EProfileSettingRingingToneLine2Id ) &&
        ( identifier != EProfileSettingVideoCallToneId ) &&
        ( identifier != EProfileSettingMessageAlertToneId ) &&
        ( identifier != EProfileSettingEmailAlertToneId ) &&
        ( identifier != EProfileSettingPTTRingingToneId ) )
        {
        if( aCalledFromMenu ||
            !( identifier == EProfileSettingVibratingAlertId ||
               identifier == EProfileSettingEmailVibratingAlertId ||
               identifier == EProfileSettingWarningAndGameTonesId ||
               identifier == EProfileSettingTTSRingingToneId ) )
            {
            // Set profile name as title pane text.
            // Tone selections launch a pop-up, so title pane is not changed.
            // Binary pop-up setting item with two available values does not
            // launch a setting page when not called from menu so title pane
            // is not changed.
            if( iTitlePane )
                {
                titleText = iTitlePane->Text()->AllocLC();
                SetTitlePaneTextL( iCopyProfile->iTitleName );
                }
            }
        }

    
    TInt threeDEffect = iCopyProfile->i3DEffect;
    if( identifier == EProfileSetting3dEffectId && iIndexHandler )
    	{
    	iIndexHandler->StoreIndices();
    	}

    // Set boolean value to ETrue that we know that the settings container
    // is in editing mode.
    iItemEdited = ETrue;
    // Store currently active profile id
    TInt activeProfileId( iEngineHandler.Engine()->ActiveProfileId() );

    // Trap to set boolean value back to EFalse after EditItemL finalized.
    PRODEBUG1( "CProfileSettingsContainer EditItemL before list::EditItemL(%d)",aIndex );
    TRAPD( error, CAknSettingItemList::EditItemL( aIndex, aCalledFromMenu ) );
    PRODEBUG1( "CProfileSettingsContainer EditItemL after list::EditItemL(),error=%d",error );
    iItemEdited = EFalse;
    User::LeaveIfError( error );

    // We don't know whether user cancelled or oked the setting so we must
    // store the data from setting item to the modified profile (iCopyProfile).
    item->StoreL();
    // Compare the settings of the modified profile
    // to the settings currently in Profile Engine.
    if( !iCopyProfile->CompareL( iProfile ) )
        {
        // Handle removing of DRM automated content
        iCopyProfile->RemoveAutomatedContentL( iProfile );
        // Something has changed. Store settings of modified profile.
        iCopyProfile->StoreL( iProfile );
        // Trap saving of the profile. If for example flash memory runs out
        // we can still revert the UI to its previous state.
        TRAP( error, iEngineHandler.Engine()->CommitChangeL( *iProfile ) );
        if( error != KErrNone || PowerSaveMode() )
            {
            // An User::Leave occurred. Try to read old settings to memory.
            ReadProfileL();
            // Update setting item so UI is left to correct state.
            item->LoadL();
            item->UpdateListBoxTextL();
            }
  		else if( identifier ==  EProfileSettingProfileNameId )
            {
            // Profile name was changed.
            UpdateProfileNamesL();
            }
            
        if ( threeDEffect != iCopyProfile->i3DEffect )
            {
            UpdateSettingsL( EFalse );  // hide 3DEcho if 3DEffect is off
            }
        }
    // If the setting is an external (= only in Tones View):
    iExternalSettingsHandler.StoreIfChangedL( identifier );

    if( titleText )
        {
        // Set title pane text back
        SetTitlePaneTextL( *titleText );
        CleanupStack::PopAndDestroy( titleText );
        }
    // Compare active profile to previously active profile
    TInt newActiveProfileId( iEngineHandler.Engine()->ActiveProfileId() );
    if( activeProfileId != newActiveProfileId )
        {
        // Active profile has changed while editing. Update UI.
        HandleActiveProfileEventL( EProfileNewActiveProfile, newActiveProfileId );
        }
    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::CreateSettingItemL
// -----------------------------------------------------------------------------
//
CAknSettingItem* CProfileSettingsContainer::CreateSettingItemL(
    TInt aIdentifier )
    {
    PRODEBUG1( "CProfileSettingsContainer CreateSettingsItemL(%d)",aIdentifier );
    CAknSettingItem* settingItem = NULL;

    switch( aIdentifier )
        {
        // Flow through: Ringing tone and RT line 1 are stored to the same
        // variable because they can not exist at the same time.
        case EProfileSettingRingingToneId:
        case EProfileSettingRingingToneLine1Id:
            {
            settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iRingingTone, aIdentifier );
            break;
            }
        case EProfileSettingRingingToneLine2Id:
            {
            settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iRingingToneLine2, aIdentifier );
            break;
            }
        case EProfileSettingVideoCallToneId:
            {
            settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iVideoCallTone, aIdentifier );
            break;
            }
        case EProfileSettingMessageAlertToneId:
            {
            settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iMessageAlertTone, aIdentifier );
            break;
            }
        case EProfileSettingEmailAlertToneId:
            {
            settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iEmailAlertTone, aIdentifier );
            break;
            }
        case EProfileSettingRingingTypeId:
            {
            settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->iRingingType );
            break;
            }
        case EProfileSettingRingingVolumeId:
            {
            settingItem = new( ELeave ) CProfilePlayingVolumeSettingItem(
                aIdentifier, iCopyProfile->iRingingVolume,
                *iCopyProfile->iRingingTone,
                iCopyProfile->iRingingType,
                iCopyProfile->iVibratingAlert,
                EFalse ); //Display confirmation note for maximum volume
            break;
            }
		 case EProfileSettingMediaVolumeId:
            {
            settingItem = new( ELeave ) CProfilePlayingVolumeSettingItem(
                aIdentifier, iCopyProfile->iMediaVolume,
                *iCopyProfile->iRingingTone,
                iCopyProfile->iRingingType,
                iCopyProfile->iVibratingAlert,
                EFalse ); //Do not display confirmation note for maximum volume
            break;
            }
        case EProfileSettingVibratingAlertId:
            {
            settingItem = new( ELeave ) CAknBinaryPopupSettingItem(
                aIdentifier, iCopyProfile->iVibratingAlert );
            break;
            }
        case EProfileSettingEmailVibratingAlertId:
            {
            settingItem = new( ELeave ) CAknBinaryPopupSettingItem(
                aIdentifier, iCopyProfile->iEmailVibratingAlert );
            break;
            }
        case EProfileSettingKeypadVolumeId:
            {
            settingItem = new( ELeave ) CProfileKeypadVolumeSettingItem(
                aIdentifier, iCopyProfile->iKeypadVolume, iKeySounds );
            break;
            }
        case EProfileSettingWarningAndGameTonesId:
            {
            settingItem = new( ELeave ) CAknBinaryPopupSettingItem(
                aIdentifier, iCopyProfile->iWarningAndGameTones );
            break;
            }
        case EProfileSettingAlertForId:
            {
            settingItem = new( ELeave ) CProfileAlertForSettingItem(
                aIdentifier, iCopyProfile->iContactList, iCoeEnv );
            break;
            }
        case EProfileSettingProfileNameId:
            {
            settingItem = new( ELeave ) CProfileNameSettingItem(
                aIdentifier, iCopyProfile->iProfileName, iEngineHandler );
            break;
            }
		case EProfileSettingTTSRingingToneId:
			{
			settingItem = new( ELeave ) CAknBinaryPopupSettingItem(
                aIdentifier, iCopyProfile->iTextToSpeech );
			break;
			}
		case EProfileSettingPTTRingingToneId:
			{
		 	settingItem = CreateFileListSettingItemL(
                *iCopyProfile->iPTTRingingTone, aIdentifier );
			break;
			}
		case EProfileSettingPTTStatusId:
			{
			settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->iPTTStatus );
			break;
			}
		case EProfileSettingTactileFeedbackId:
            {
            /*
            settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->iTactileFeedback );
            */
            settingItem = new( ELeave ) CProfileFeedbackSettingItem(
                aIdentifier, iCopyProfile->iTactileFeedback );
            break;
            }    
		case EProfileSettingAudioFeedbackId:
            {
            /*
            settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->iAudioFeedback );
            */
            settingItem = new( ELeave ) CProfileFeedbackSettingItem(
                aIdentifier, iCopyProfile->iAudioFeedback );
            break;
            }
		case EProfileSetting3dEffectId:
            {
            /*
            settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->i3DEffect );
            */
            settingItem = new( ELeave ) CProfile3dSettingItem(
                aIdentifier,
                iCopyProfile->i3DEffect,
                *iCopyProfile->iRingingTone,
                iCopyProfile->iRingingType,
                iCopyProfile->iVibratingAlert,
                iCopyProfile->iRingingVolume,
                iCopyProfile->i3DEcho );
            break;
            }    
		case EProfileSetting3dEchoId:
            {
            /*
            settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem(
                aIdentifier, iCopyProfile->i3DEcho );
            */
            settingItem = new( ELeave ) CProfile3dSettingItem(
                aIdentifier,
                iCopyProfile->i3DEcho,
                *iCopyProfile->iRingingTone,
                iCopyProfile->iRingingType,
                iCopyProfile->iVibratingAlert,
                iCopyProfile->iRingingVolume,
                iCopyProfile->i3DEffect );
            break;
            }
			
        default:
            {
            PRODEBUG1( "CProfileSettingsContainer before extShandler::CreateSI(%d)",aIdentifier );
            settingItem = iExternalSettingsHandler.CreateSettingItemL( aIdentifier );
            PRODEBUG( "CProfileSettingsContainer after extShandler::CreateSI" );
            break;
            }
        }
    PRODEBUG1( "CProfileSettingsContainer CreateSettingsItemL(%d) before return",aIdentifier );
    return settingItem;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::CheckFileExistenceL
// -----------------------------------------------------------------------------
//  
void CProfileSettingsContainer::CheckFileExistenceL()
	{
	 CAknSettingItemArray* array = SettingItemArray();
	            
	if( !IsItemHidden( EProfileSettingRingingToneId, array ) )
		{
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iRingingTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iRingingTone );
        	HBufC* ringtoneNotFoundText =
        		StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
		
	if( !IsItemHidden( EProfileSettingRingingToneLine1Id, array ) )
		{
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iRingingTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iRingingTone );
        	HBufC* ringtoneNotFoundText =
        		StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}	
	
	if( !IsItemHidden( EProfileSettingRingingToneLine2Id, array ) )
		{
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iRingingToneLine2 ) )
			{
			TParsePtrC tone( *iCopyProfile->iRingingToneLine2 );
        	HBufC* ringtoneNotFoundText =
        		StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

			CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
			CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
	
	if( !IsItemHidden( EProfileSettingVideoCallToneId, array ) )
		{		
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iVideoCallTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iVideoCallTone );
    		HBufC* ringtoneNotFoundText =
    			StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
	
	if( !IsItemHidden( EProfileSettingMessageAlertToneId, array ) )
		{	
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iMessageAlertTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iMessageAlertTone );
    		HBufC* ringtoneNotFoundText =
    			StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
	
	if( !IsItemHidden( EProfileSettingEmailAlertToneId, array ) )
		{	
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iEmailAlertTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iEmailAlertTone );
    		HBufC* ringtoneNotFoundText =
    			StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
	
	if( !IsItemHidden( EProfileSettingPTTRingingToneId, array ) )
		{	
		if( !BaflUtils::FileExists( *iFs, *iCopyProfile->iPTTRingingTone ) )
			{
			TParsePtrC tone( *iCopyProfile->iPTTRingingTone );
    		HBufC* ringtoneNotFoundText =
    			StringLoader::LoadLC(
        		R_PROFILE_CONFIRM_RINGTONE_FOUND, tone.NameAndExt() );

	    	CProfileSettingsContainer::LaunchDialogL( *ringtoneNotFoundText );
        	CleanupStack::PopAndDestroy( ringtoneNotFoundText );
			}
		}
	}

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::LaunchDialogL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::LaunchDialogL( const TDesC& aTitleText )
	{
	CAknQueryDialog* query = CAknQueryDialog::NewL();
    	const TInt ret( query->ExecuteLD(
    	 R_PROFILE_CONFIRM_RINGTONE_FOUND_QUERY, aTitleText) );
	}

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::CreateFileListSettingItemL
// -----------------------------------------------------------------------------
//
CAknSettingItem* CProfileSettingsContainer::CreateFileListSettingItemL(
    TDes& aFileName, TInt aIdentifier )
    {
    CAknSettingItem* settingItem = new( ELeave ) CProfileFileListSettingItem(
        aIdentifier,
        aFileName,
        iCopyProfile->iRingingType,
        iCopyProfile->iRingingVolume,
        iCopyProfile->iVibratingAlert,
        iCopyProfile->i3DEffect,
        iCopyProfile->i3DEcho,
        iCoeEnv );

    return settingItem;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::IndexForId
// -----------------------------------------------------------------------------
//
TInt CProfileSettingsContainer::IndexForId( TInt aId ) const
    {
    CAknSettingItemArray* array = SettingItemArray();
    TInt count( array->Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        if( array->At( index )->Identifier() == aId )
            {
            // Searched ID was found, return index
            return index;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::SetItemHidden
// Sets an item hidden if an index for the id is found.
// In other words does nothing if feature is not supported.
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::SetItemHidden( TInt aId,
    CAknSettingItemArray* aArray, TBool aHidden )
    {
    TInt index( IndexForId( aId ) );
    if( index == KErrNotFound )
        {
        return;
        }
    aArray->At( index )->SetHidden( aHidden );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::IsItemHidden
// 
// -----------------------------------------------------------------------------
//
TBool CProfileSettingsContainer::IsItemHidden( TInt aId,
    CAknSettingItemArray* aArray ) const
    {
    TInt index( IndexForId( aId ) );
    if( index == KErrNotFound )
        {
        return ETrue;
        }
        
    return aArray->At( index )->IsHidden();
    }    

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::SetTitlePaneTextL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::SetTitlePaneTextL( const TDesC& aTitle )
	{
    if( !iTitlePane )
        {
        return;
        }

    if( aTitle.Length() > 0 )
		{
		iTitlePane->SetTextL( aTitle );
		}
	else
		{
		iTitlePane->SetTextToDefaultL(); // Application name
		}
	}

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::UpdateProfileNamesL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::UpdateProfileNamesL()
    {
    if( iIndexHandler )
        {
        TInt newProfileIndex( iIndexHandler->ReadIdArrayAndUpdateL() );
        if( iTabHandler )
            {
            // Refresh tabs with changed profile names
            iTabHandler->RefreshTabsL();
            iTabHandler->SetActiveTab( newProfileIndex );
            }
        else
            {
            // If the iTabHandler is NULL and the iNaviPaneContainer is not NULL
            // Set the text of navi pane to currently personlize profile's name.
            if ( iNaviPaneContainer )
                {
                MProfilesNamesArray* nameArray = iEngineHandler.IdArray();
                iNaviPaneContainer->SetNaviPaneTextL( 
                        nameArray->MdcaPoint( newProfileIndex ) );
                }
            }
        }
    else
        {
        // No index handler. Just refresh profile name array.
        iEngineHandler.ReadIdArrayL();
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::SizeChanged
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::SizeChanged()
    {
    CEikListBox* lb = ListBox();
    if( lb )
        {
        lb->SetRect( Rect() );  // Set container's rect to listbox
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::HandleListBoxEventL
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::HandleListBoxEventL(
    CEikListBox* aListBox, TListBoxEvent aEventType )
    {
    if( aEventType == EEventEnterKeyPressed ||
        aEventType == EEventItemDoubleClicked )
        {
        CAknSettingItemArray* array = SettingItemArray();
        if( !AllowedToChange( array->At( array->ItemIndexFromVisibleIndex(
              ListBox()->CurrentItemIndex() ) )->Identifier() ) )
            {
            // If a setting is not allowed to change, "swallow" list box event.
            return;
            }
        }
    CAknSettingItemList::HandleListBoxEventL( aListBox, aEventType );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::GetHelpContext
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidProfileApp;
    if( !iExternalSettingsHandler.GetHelpContext( aContext ) )
        {
        aContext.iContext = KMODE_HLP_PERSONALISE;
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::PhoneSettingChanged
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::PhoneSettingChanged(
    TSSSettingsSetting aSetting, TInt aNewValue )
    {
    if( aSetting == ESSSettingsAls )
        {
        iAlternateLineService = ( aNewValue != ESSSettingsAlsNotSupported );
        if( iIndexHandler )
            {
            iIndexHandler->StoreIndices();
            }
        // error ignored. If not enough memory to refresh the UI,
        // don't bother launching an error note.
        TRAP_IGNORE( SetHiddenFlagsL() );
        if( iIndexHandler )
            {
            iIndexHandler->LoadIndices();
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::TabChangedL
// When a tab changes, this method gets called.
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::TabChangedL( TInt /* aIndex */ )
    {
    UpdateSettingsL();

    CEikListBox* listBox = ListBox();
    if( listBox )
    	{
    	listBox->UpdateScrollBarsL();
    	}
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::HandleActiveProfileEventL
// When active profile changes, this method gets called.
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::HandleActiveProfileEventL(
    TProfileEvent /* aProfileEvent */, TInt /* aProfileId */ )
    {
    /*
    if( iItemEdited )
        {
        // Do not update UI if an item is being edited.
        return;
        }
    */
    UpdateProfileNamesL();
    UpdateSettingsL();
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::HandleExternalSettingsChangedL
// When external settings change, this method gets called.
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::HandleExternalSettingsChangedL()
    {
    UpdateSettingsL( EFalse );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::HandleResourceChange
// -----------------------------------------------------------------------------
//
void CProfileSettingsContainer::HandleResourceChange( TInt aType )
    {
    if ( aType == KEikDynamicLayoutVariantSwitch ||
         aType == KAknsMessageSkinChange )
        {
        iProfileViewHandler.UpdateClientRect();
        }
    CAknSettingItemList::HandleResourceChange( aType );
    }

// ----------------------------------------------------
//  CProfileSettingsContainer::HandleNotifyInt
// ----------------------------------------------------
//
void CProfileSettingsContainer::HandleNotifyInt(
    const TUint32 aID,
    const TInt aNewValue )
    {
    if ( KDynamicVoIP == aID)
        {
        iVoIPStatus = KVoIPOFF != aNewValue;
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::ItemEdited
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CProfileSettingsContainer::ItemEdited() const
    {
    return iItemEdited;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsContainer::PowerSaveMode
//
// -----------------------------------------------------------------------------
//
TBool CProfileSettingsContainer::PowerSaveMode( TInt aSettingId )
    {
    TInt ret = EFalse;
    
    if ( aSettingId == EProfileSettingVibratingAlertId ||
         aSettingId == EProfileSettingKeypadVolumeId ||
         aSettingId == EProfileSettingTactileFeedbackId ||
         aSettingId == EProfileSettingAudioFeedbackId ||aSettingId == EProfileSettingEmailVibratingAlertId)
        {
      	ret = PowerSaveMode();
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CProfileSettingsContainer::PowerSaveModeL
//
// -----------------------------------------------------------------------------
//
TBool CProfileSettingsContainer::PowerSaveModeL()
    {
    if( !FeatureManager::FeatureSupported( KFeatureIdPowerSave ) )
        {
        return EFalse;
        }

    TInt psm = EPsmsrvModeNormal;
    
    // KProEngPsmChanged PubSub key is set by GSProfilesPlugin when power save
    // mode changes. If the key cannot be read (doesn't exist), PSM hasn't
    // changed since the last boot and the mode is read from PSM CenRep.
    if( RProperty::Get( KPSUidProfileEngine, KProEngPsmChanged, psm ) != KErrNone )
        {
        CRepository* cenrep = CRepository::NewLC( KCRUidPowerSaveMode );
        User::LeaveIfError( cenrep->Get( KPsmCurrentMode, psm ) );
        CleanupStack::PopAndDestroy( cenrep );
        }

    if ( psm == EPsmsrvModePowerSave )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CProfileSettingsContainer::PowerSaveMode
//
// -----------------------------------------------------------------------------
//
TBool CProfileSettingsContainer::PowerSaveMode()
    {
    TBool psm = EFalse;
    
    TRAPD( err, psm = PowerSaveModeL() );
    
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    return psm;
    }

    

// End of File
