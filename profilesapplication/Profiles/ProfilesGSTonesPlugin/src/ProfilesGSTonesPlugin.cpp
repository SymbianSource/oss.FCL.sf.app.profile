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
* Description:    Cpp file for plugin.
*
*/


// Includes
#include "ProfilesGSTonesPlugin.h"

#include "ProfilesUID.h"
#include "CProfilesGSTonesPluginSettingsHandler.h"
#include "ProfilesDebug.h"
#include "ProfilesGSTonesPlugin.hrh"
#include <profilesgstonespluginrsc.rsg>
#include <CProfileNaviPaneContainer.h>

// Includes from Profiles Settings View:
#include <CProfileSettingsContainer.h>
#include <CProfileEngineHandler.h>

// Includes from Profiles Engine:
#include <MProfileEngineExtended.h>
#include <MProfilesLocalFeatures.h>
#include <CProfileChangeNotifyHandler.h>
#include "ProfilesVariant.hrh"

// Includes from GS framework:
#include <GSFWViewUIDs.h>
#include <GSPrivatePluginProviderIds.h>

// External includes
#include <StringLoader.h>
#include <bautils.h>
#include <f32file.h>
#include <data_caging_path_literals.hrh>
#include <FeatMgr.h>
#include <hlplch.h>
#include <akntitle.h>
#include <aknglobalnote.h>

class CProfileIndexHandler;
class CProfileTabHandler;

namespace {
// Constants
    const TUid KProfilesGSTonesPluginUid =
        { IMPLEMENTATION_UID_PROFILES_GS_TONES_PLUGIN };

    _LIT( KProfilesGSTonesPluginResourceFileName,
          "z:ProfilesGSTonesPluginRsc.rsc" );
}

// CLASS DECLARATION


/******************************************************************************
 * class CProfileGSTonesTopControl
 ******************************************************************************/


CProfileGSTonesTopControl* CProfileGSTonesTopControl::NewL( CCoeControl* aFocusControl )
    {
    CProfileGSTonesTopControl* self = new ( ELeave ) CProfileGSTonesTopControl();
    self->iFocusControl = aFocusControl;
    return self;
    }

void CProfileGSTonesTopControl::FocusChanged( TDrawNow aDrawNow )
    {
    // Not calling super::FocusChanged is intentional
    if( iFocusControl )
        {
        iFocusControl->SetFocus( IsFocused(), aDrawNow );
        }
    }


/******************************************************************************
 * class CProfilesGSTonesPlugin
 ******************************************************************************/



// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::CProfilesGSTonesPlugin()
// ---------------------------------------------------------------------------
//
CProfilesGSTonesPlugin::CProfilesGSTonesPlugin()
    : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::~CProfilesGSTonesPlugin()
// ---------------------------------------------------------------------------
//
CProfilesGSTonesPlugin::~CProfilesGSTonesPlugin()
    {
    DoDeactivate();
    delete iEngineHandler;
    iResourceLoader.Close();
    delete iNotifier;
    }


// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::ConstructL()
// ---------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::ConstructL()
    {
    PRODEBUG( "CProfilesGSTonesPlugin::ConstructL() begin" );
    // Find the resource file
    TParse parse;
    parse.Set( KProfilesGSTonesPluginResourceFileName, 
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );
    
    PRODEBUG( "CProfilesGSTonesPlugin::ConstructL() before BaseConstructL call" );
    BaseConstructL( R_PROFILES_GS_TONES_VIEW ); // CSI: 9 # The resource file must be opened before this call

    iNotifier = CProfileChangeNotifyHandler::NewL( this );
    iEngineHandler = CProfileEngineHandler::NewL();
    iPreviousProfile = iEngineHandler->Engine()->ActiveProfileId();
    PRODEBUG( "CProfilesGSTonesPlugin::ConstructL() end" );
    }


// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::NewL()
// ---------------------------------------------------------------------------
//
CProfilesGSTonesPlugin* CProfilesGSTonesPlugin::NewL( TAny* /*aInitParams*/ )
	{
	CProfilesGSTonesPlugin* self = new(ELeave) CProfilesGSTonesPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


// ---------------------------------------------------------------------------
// CGSParentPlugin::Id()
// ---------------------------------------------------------------------------
//
TUid CProfilesGSTonesPlugin::Id() const
    {
    PRODEBUG( "CProfilesGSTonesPlugin::Id" );
    return KProfilesGSTonesPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
// ---------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::DoActivateL
    ( const TVwsViewId& /* aPrevViewId */,
      TUid /* aCustomMessageId */,
      const TDesC8& /* aCustomMessage */ )
    {
    PRODEBUG( "CProfilesGSTonesPlugin::DoActivateL begin" );
    if ( !iContainer )
        {
        iSettingsHandler =
            CProfilesGSTonesPluginSettingsHandler::NewL( iCoeEnv );

        iContainer = CProfileSettingsContainer::NewL(
            *this, *iEngineHandler, *iSettingsHandler, (CProfileIndexHandler*) NULL,
            (CProfileTabHandler*) NULL, (CAknTitlePane*) NULL,
            R_PROFILE_GS_TONES_VIEW_SETTING_ITEM_LIST );

        iTopControl = CProfileGSTonesTopControl::NewL( iContainer );

        // Set the title pane text
       	CAknTitlePane* titlePane = static_cast< CAknTitlePane* >(
		    StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        HBufC* title = StringLoader::LoadLC( R_PROFILES_GS_TONES_VIEW_TITLE );
        titlePane->SetTextL( *title );
        CleanupStack::PopAndDestroy( title );

        iNaviPaneContainer =
            CProfileNaviPaneContainer::NewL( *StatusPane(), *iEngineHandler );
        // Set the name of the active profile to navi pane
        iNaviPaneContainer->SetNaviPaneTextL();

        AppUi()->AddToViewStackL( *this, iContainer );
        // Top control is always on top as it listens to focus changes (which
        // will be routed to container to adapt highlight animation to focus)
        AppUi()->AddToStackL( *this, iTopControl );

        iContainer->MakeVisible( ETrue );
        }
    PRODEBUG( "CProfilesGSTonesPlugin::DoActivateL end" );
    }

// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
// ---------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::DoDeactivate()
    {
    PRODEBUG( "CProfilesGSTonesPlugin::DoDeactivateL begin" );
    delete iNaviPaneContainer;
    iNaviPaneContainer = NULL;

    if ( iTopControl )
        {
        AppUi()->RemoveFromStack( iTopControl );
        delete iTopControl;
        iTopControl = NULL;
        }
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    delete iSettingsHandler;
    iSettingsHandler = NULL;
    PRODEBUG( "CProfilesGSTonesPlugin::DoDeactivateL end" );
    }

// ----------------------------------------------------------------------------
// CProfilesGSTonesPlugin::HandleCommandL
// 
// Handles commands directed to this class.
// ----------------------------------------------------------------------------
void CProfilesGSTonesPlugin::HandleCommandL( TInt aCommand )
    {   
    PRODEBUG1( "CProfilesGSTonesPlugin::HandleCommandL(%d) begin", aCommand );
    CAknViewAppUi* appUi = AppUi();
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            appUi->ActivateLocalViewL( KGSPrslnPluginUid ); 
            break;
        case EProfilesGsTonesCmdChange:
        case EAknSoftkeyChange:
            PRODEBUG( "CProfilesGSTonesPlugin::HandleCommandL before EditItemL" );
            iContainer->EditItemL(
                iContainer->SettingItemArray()->ItemIndexFromVisibleIndex(
                    iContainer->ListBox()->CurrentItemIndex() ),
                    ( aCommand == EProfilesGsTonesCmdChange ) );
            PRODEBUG( "CProfilesGSTonesPlugin::HandleCommandL after EditItemL" );
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), appUi->AppHelpContextL() );
                }
            break;
            }
        default:
            appUi->HandleCommandL( aCommand );
            break;
        }
    PRODEBUG( "CProfilesGSTonesPlugin::HandleCommandL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPlugin::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::DynInitMenuPaneL(
    TInt aResourceId, CEikMenuPane* aMenuPane )
	{
	if( aResourceId == R_PROFILES_GS_OPTIONS_MENUPANE )
		{
		if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
			{
			// Help is not supported. Hide Help from Options menu
			aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
			}
		}
	}

// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::GetCaptionL()
// ---------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::GetCaptionL( TDes& aCaption ) const
    {
    PRODEBUG( "CProfilesGSTonesPlugin::GetCaptionL begin" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_PROFILES_GS_TONES_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    PRODEBUG( "CProfilesGSTonesPlugin::GetCaptionL end" );
    }

// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::PluginProviderCategory()
// ---------------------------------------------------------------------------
//
TInt CProfilesGSTonesPlugin::PluginProviderCategory() const
    {
    PRODEBUG( "CProfilesGSTonesPlugin::PluginProviderCategory" );
    return KGSPluginProviderInternal;
    }

// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::ItemType()
// ---------------------------------------------------------------------------
//
TGSListboxItemTypes CProfilesGSTonesPlugin::ItemType()
    {
    PRODEBUG( "CProfilesGSTonesPlugin::ItemType" );
    return EGSItemTypeSetting;
    }

// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::UpdateClientRect()
// ---------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::UpdateClientRect()
    {
    PRODEBUG( "CProfilesGSTonesPlugin::UpdateClientRect" );
	iContainer->SetRect( ClientRect() );
    }

// ---------------------------------------------------------------------------
// CProfilesGSTonesPlugin::View()
// ---------------------------------------------------------------------------
//
CAknView& CProfilesGSTonesPlugin::View()
    {
    PRODEBUG( "CProfilesGSTonesPlugin::View" );
    return *this;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPlugin::HandleActiveProfileEventL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::HandleActiveProfileEventL(
    TProfileEvent aProfileEvent, TInt aProfileId )
    {
    PRODEBUG( "CProfilesGSTonesPlugin::HandleActiveProfileEventL" );
    if( ( aProfileEvent == EProfileNewActiveProfile ) &&
        ( aProfileId != iPreviousProfile ) &&
        iNaviPaneContainer )
        {
        // Active profile has changed. Change active profile name in navi pane.
        iNaviPaneContainer->SetNaviPaneTextL();
        ShowInfoNoteL( R_PROFILES_GS_TONES_VIEW_UPDATED_NOTE );
        iPreviousProfile = aProfileId;
        }
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPlugin::ShowInfoNoteL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPlugin::ShowInfoNoteL( TInt aResourceId )
    {
    if( IsForeground() )
        {
        HBufC* infoText = StringLoader::LoadLC( aResourceId );
        CAknGlobalNote* note = CAknGlobalNote::NewLC();
        note->ShowNoteL( EAknGlobalInformationNote, *infoText );

        CleanupStack::PopAndDestroy( 2, infoText );
        }
    }

// End of File

