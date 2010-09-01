/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSProfilesPlugin implementation.
*
*/


// User includes
#include    "GSProfilesPlugin.h"
#include    <gsparentplugin.h>
#include    <gscommon.hrh>
#include    <gsprofilespluginrsc.rsg> // GUI Resource
#include    <gsprofilesplugin.mbg>
#include    <gsprivatepluginproviderids.h>
#include    <ProfileEngineSDKCRKeys.h>
#include    <hwrmvibrasdkcrkeys.h>

// System includes
#include    <apadoc.h>
#include    <AknNullService.h>
#include    <bautils.h>
#include    <StringLoader.h>
#include    <centralrepository.h>

#include    <ProEngFactory.h>
#include    <MProfileEngine.h>
#include    <MProfile.h>
#include    <MProfileTones.h>
#include    <MProEngToneSettings.h>
#include    <MProfileExtraSettings.h>
#include    <MProfileFeedbackSettings.h>
#include    <TProfileToneSettings.h>

#include    <ProfileEng.hrh>
#include    <ProfileEngineInternalCRKeys.h>

// PSM client
#include <psmclient.h>
#include <psmsettings.h>
#include <e32property.h>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <ProfileEnginePrivatePSKeys.h>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSProfilesPlugin::CGSProfilesPlugin
//
// ---------------------------------------------------------------------------
//
CGSProfilesPlugin::CGSProfilesPlugin()
    : iResources( *iCoeEnv ), iNullService(NULL)
    {
    }


// ---------------------------------------------------------------------------
// CGSProfilesPlugin::~CGSProfilesPlugin
//
// ---------------------------------------------------------------------------
//
CGSProfilesPlugin::~CGSProfilesPlugin()
    {
    iResources.Close();

    if ( iNullService )
        {
        delete iNullService;
        }
    }


// ---------------------------------------------------------------------------
// CGSProfilesPlugin::ConstructL
//
// ---------------------------------------------------------------------------
//
void CGSProfilesPlugin::ConstructL()
    {
    OpenLocalizedResourceFileL( KGSProfilesPluginResourceFileName, iResources );
    }


// ---------------------------------------------------------------------------
// CGSProfilesPlugin::NewL
//
// ---------------------------------------------------------------------------
//
CGSProfilesPlugin* CGSProfilesPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSProfilesPlugin* self = new ( ELeave ) CGSProfilesPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::Id (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
TUid CGSProfilesPlugin::Id() const
    {
    return KGSProfilesPluginImplUID;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::DoActivateL (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::DoDeactivate (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::DoDeactivate()
    {
    }



// -----------------------------------------------------------------------------
// CGSProfilesPlugin::GetCaptionL (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_PROFILES_PLUGIN_CAPTION );
    
    if (result->Des().Length() < aCaption.MaxLength())
        {
        aCaption.Copy( *result );    
        }
    else
        {
        aCaption = KNullDesC;
        }
   
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::PluginProviderCategory (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
TInt CGSProfilesPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::ItemType (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSProfilesPlugin::ItemType()
    {
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::GetValue (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::GetValue( const TGSPluginValueKeys /*aKey*/,
                                      TDes& /*aValue*/ )
    {
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::HandleSelection (from CGSPluginInterface)
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::HandleSelection(
    const TGSSelectionTypes /*aSelectionType*/ )
    {
    TRAP_IGNORE( LaunchProfilesAppL() );
    }

// ---------------------------------------------------------------------------
// CGSProfilesPlugin::CreateIconL (from CGSPluginInterface)
//
// ---------------------------------------------------------------------------
//

CGulIcon* CGSProfilesPlugin::CreateIconL( const TUid aIconType )
    {
    CGulIcon* icon;
    TParse* fp = new ( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSProfilesPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if ( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpPersoProf,
        fp->FullName(),
        EMbmGsprofilespluginQgn_prop_cp_perso_prof,
        EMbmGsprofilespluginQgn_prop_cp_perso_prof_mask );
        }    
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::OpenLocalizedResourceFileL
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::OpenLocalizedResourceFileL(
    const TDesC& aResourceFileName,
    RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );
    CleanupClosePushL(fsSession);

    // Find the resource file:
    TParse parse;
    parse.Set( aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    aResourceLoader.OpenL( fileName );

    CleanupStack::PopAndDestroy(&fsSession);
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::LaunchProfilesAppL
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::LaunchProfilesAppL()
    {
    // Get the correct application data
    RWsSession ws;
    User::LeaveIfError(ws.Connect());
    CleanupClosePushL(ws);

    // Find the task with uid
    TApaTaskList taskList(ws);
    TApaTask task = taskList.FindApp( KGsProfilesAppUid );

    if ( task.Exists() )
        {
        task.BringToForeground();
        }
    else
        {
        //Launch Profiles application as embedded
        TAppInfo app( KGsProfilesAppUid, KGSProfilesApp );
        iEmbedded=NULL;
        EmbedAppL( app );
        }
    CleanupStack::PopAndDestroy(&ws);
    }


// -----------------------------------------------------------------------------
// CGSProfilesPlugin::EmbedAppL
//
// -----------------------------------------------------------------------------
//
void CGSProfilesPlugin::EmbedAppL( const TAppInfo& aApp )
    {
    // Launch settings app
    if ( iNullService )
        {
        delete iNullService;
        iNullService = NULL;
        }
    iNullService = CAknNullService::NewL( aApp.iUid, this );
    }



/******************************************************************************
 * class CPSMProfilesPlugin                  
 ******************************************************************************/


//
// config file goes to:
// \epoc32\release\winscw\udeb\Z\private\2000B187 (emulator)
// \epoc32\data\Z\private\2000B187  (image)
//



// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::CPSMProfilesPlugin()
// ----------------------------------------------------------------------------------
//  
CPSMProfilesPlugin::CPSMProfilesPlugin( TPsmPluginCTorParams& aInitParams ) :
    CPsmPluginBase( aInitParams )
	{	
	}

// -----------------------------------------------------------------------------
// CPSMProfilesPlugin::ConstructL(
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSMProfilesPlugin::ConstructL()
    {
    }
    

// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::NewL() 
// ----------------------------------------------------------------------------------
//  
CPSMProfilesPlugin* CPSMProfilesPlugin::NewL( TPsmPluginCTorParams& aInitParams )
	{
	CPSMProfilesPlugin* self = new ( ELeave ) CPSMProfilesPlugin( aInitParams );

	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
	}
	

// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::~CPSMProfilesPlugin()
// ----------------------------------------------------------------------------------
//  
CPSMProfilesPlugin::~CPSMProfilesPlugin()
	{
	}



// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::NotifyModeChange (from CPsmPluginBase)
// ----------------------------------------------------------------------------------
//  
void CPSMProfilesPlugin::NotifyModeChange( const TInt aMode )
    {
    TInt ignoreError = KErrNone;
    
    TRAP( ignoreError, NotifyModeChangeL( aMode) );
    }


// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::NotifyModeChangeL()
// ----------------------------------------------------------------------------------
//  
void CPSMProfilesPlugin::NotifyModeChangeL( const TInt aMode )
    {
    TPsmSettingItems set;

    if ( aMode == EPsmsrvModeNormal || aMode == EPsmsrvPartialMode )
        {
        GetActiveProfileSettingsL( set );
        }
    else if ( aMode == EPsmsrvModePowerSave )
        {
        GetPsmConfigSettingsL( set );
        }
    else
        {
        User::Leave( KErrUnknown );
        }
    
    CRepository* cenrep = CRepository::NewLC( KCRUidVibraCtrl );
    User::LeaveIfError( cenrep->Set( KVibraCtrlProfileVibraEnabled, 
    		set.iVibraAlert ) );
    User::LeaveIfError( cenrep->Set( KVibraCtrlProfileFeedbackEnabled, 
    		set.iTactileFeedback ) );
    CleanupStack::PopAndDestroy( cenrep );
    
    cenrep = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( cenrep->Set( KProEngActiveKeypadVolume, 
    		set.iKeypadVolume ) );
    User::LeaveIfError( cenrep->Set( KProEngActiveAudioFeedback, 
    		set.iAudioFeedback ) );
    User::LeaveIfError( cenrep->Set( KProEngActiveTactileFeedback, 
    		set.iTactileFeedback ) );
    CleanupStack::PopAndDestroy( cenrep );

    // Set up internal PSM PubSub key
    _LIT_SECURITY_POLICY_PASS( KProfileEngReadPolicy );
    const TSecurityPolicy KProfileEngWritePolicy( ECapabilityWriteDeviceData );
    TInt err = RProperty::Define( KPSUidProfileEngine, 
    		KProEngPsmChanged, RProperty::EInt,
    		KProfileEngReadPolicy, KProfileEngWritePolicy );
    if( err == KErrNone || err == KErrAlreadyExists )
        {
        RProperty::Set( KPSUidProfileEngine, KProEngPsmChanged, aMode );

        // Notify engine about power save mode change
        MProfileEngine* profileEngine = CreateProfileEngineL();
        CleanupReleasePushL( *profileEngine );
        TInt activeProfileId = profileEngine->ActiveProfileId();
        CleanupStack::PopAndDestroy( profileEngine );

        MProfileEngineExtended* engine = CreateProfileEngineExtendedL();
        CleanupReleasePushL( *engine );
        MProfileExtended* profile = engine->ProfileL( activeProfileId );
        CleanupReleasePushL( *profile );
        
        // CommitChangeL is needed so that an event will be created for profile
        // modification listeners.
        engine->CommitChangeL( *profile );
        
        CleanupStack::PopAndDestroy( profile );
        CleanupStack::PopAndDestroy( engine );
        }

    }


// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::GetActiveProfileSettingsL()
// ----------------------------------------------------------------------------------
//
void CPSMProfilesPlugin::GetActiveProfileSettingsL( TPsmSettingItems& aSettings )
    {
    CRepository* repository = CRepository::NewLC( KCRUidProfileEngine );
    
    TInt activeProfile = KErrNotFound;
    User::LeaveIfError( repository->Get( KProEngActiveProfile, activeProfile ) );
        
    TInt num = ResolveKey( EProfileSettingIdVibra, activeProfile );
    User::LeaveIfError( repository->Get( num, aSettings.iVibraAlert ) );
    
    num = ResolveKey( EProfileSettingIdKeypadVolume, activeProfile );
    User::LeaveIfError( repository->Get( num, aSettings.iKeypadVolume ) );
    
    num = ResolveKey( EProfileSettingIdTactileFeedback, activeProfile );
    User::LeaveIfError( repository->Get( num, aSettings.iTactileFeedback ) );

    num = ResolveKey( EProfileSettingIdAudioFeedback, activeProfile );
    User::LeaveIfError( repository->Get( num, aSettings.iAudioFeedback ) );

    CleanupStack::PopAndDestroy( repository );
    
    return;
    }


// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::GetActiveProfileSettingsL()
// ----------------------------------------------------------------------------------
//
/*
void CPSMProfilesPlugin::GetActiveProfileSettings2L( TPsmSettingItems& aSettings )
    {
    MProfileEngine* profileEngine = CreateProfileEngineL();
    CleanupReleasePushL( *profileEngine );
    
    MProfile* activeProfile  = profileEngine->ActiveProfileLC();
    
    const MProfileTones& tones = activeProfile->ProfileTones();
    const TProfileToneSettings& ts = tones.ToneSettings();
    
    aSettings.iVibraAlert  = ts.iVibratingAlert;
    aSettings.iKeypadVolume = ts.iKeypadVolume;
    
    const MProfileExtraSettings& extra = activeProfile->ProfileExtraSettings();
    const MProfileFeedbackSettings& feedBack = extra.ProfileFeedbackSettings();
    aSettings.iTactileFeedback = feedBack.TactileFeedback();
    aSettings.iAudioFeedback = feedBack.AudioFeedback();

    CleanupStack::PopAndDestroy();  // profile
    CleanupStack::PopAndDestroy( profileEngine );
    }
*/

// ----------------------------------------------------------------------------------
// CPSMProfilesPlugin::GetPsmConfigSettingsL()
// ----------------------------------------------------------------------------------
//  
void CPSMProfilesPlugin::GetPsmConfigSettingsL( TPsmSettingItems& aSettings )
    {
    RConfigInfoArray arr;
    CleanupClosePushL( arr );
   
    TPsmsrvConfigInfo info;
    info.iConfigId = EPsmPluginKeyVibra;
    info.iConfigType = EConfigTypeInt;
    info.iIntValue = KErrNotFound;
    User::LeaveIfError( arr.Append( info ) );

    info.iConfigId = EPsmPluginKeyKbdVolume;
    info.iConfigType = EConfigTypeInt;
    info.iIntValue = KErrNotFound;
    User::LeaveIfError( arr.Append( info ) );

    info.iConfigId = EPsmPluginTactileFeedback ;
    info.iConfigType = EConfigTypeInt;
    info.iIntValue = KErrNotFound;
    User::LeaveIfError( arr.Append( info ) );

    info.iConfigId = EPsmPluginAudioFeedback;
    info.iConfigType = EConfigTypeInt;
    info.iIntValue = KErrNotFound;
    User::LeaveIfError( arr.Append( info ) );

    iSettingsProvider.GetSettingsL( arr, KGSProfilesPluginUID.iUid );
        
    aSettings.iVibraAlert = arr[0].iIntValue;
    aSettings.iKeypadVolume = arr[1].iIntValue;
    aSettings.iTactileFeedback = arr[2].iIntValue;
    aSettings.iAudioFeedback = arr[3].iIntValue;

    CleanupStack::PopAndDestroy( &arr );
    }


// -----------------------------------------------------------------------------
// CPSMProfilesPlugin::ResolveKey
//
// -----------------------------------------------------------------------------
//
TUint32 CPSMProfilesPlugin::ResolveKey( TUint aPartialKey, TInt aProfileId )
    {
    const TInt KProfileIdShift = 24;
    
    return ( ( aProfileId << KProfileIdShift ) | aPartialKey );
    }


// -----------------------------------------------------------------------------
// CPSMProfilesPlugin::Panic
//
// -----------------------------------------------------------------------------
//    
void CPSMProfilesPlugin::Panic( TInt aReason )
    {
	_LIT(KPanicCategory,"CPSMProfilesPlugin");
	
	User::Panic(KPanicCategory, aReason); 
    }




// End of file
