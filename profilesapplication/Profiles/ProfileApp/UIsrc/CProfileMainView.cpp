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
* Description:  Main view class.
*
*/

// INCLUDE FILES
#include    "CProfileMainView.h"

#include    <aknViewAppUi.h>
#include    <aknnotewrappers.h>
#include    <eikmenub.h>
#include    <featmgr.h>
#include    <AknQueryDialog.h>

#ifndef RD_STARTUP_CHANGE
#include	<PSVariables.h>
#include	<e32property.h>
#else
#include 	<startupdomainpskeys.h>
#endif 

#include    <akntitle.h>
#include	<StringLoader.h>
#include    <CProfileChangeNotifyHandler.h>
#include    <CProfileEngineHandler.h>
#include    <CProfileIndexHandler.h>
#include	<MProfileEngine.h>
#include    <MProfileEngineExtended.h>
#include    <MProfile.h>
#include    <MProfileName.h>
#include    <MProfileExtended.h>
#include    <MProfilesNamesArray.h>
#include    <Profile.hrh>
#include    <profileapp.rsg>
#include 	<AknQueryDialog.h>

#include    "CProfileMainContainer.h"
#include    "CProfileDocument.h"
#include    "ProfileApp.hrh"
#include    "CProfileNaviPaneContainer.h"
#include    "MProfilesLocalFeatures.h"
#include 	"ProfilesVariant.hrh" // KProEngFeatureIdTimedProfiles


// CONSTANTS
namespace
	{
	//const TInt KNumberOfVisibleProfilesInMainView( 6 );
	const TInt KOneMinuteInMicrosecond = 1000000 * 60;
	const TInt KOneHourInMinute = 60;
	}


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileMainView::CProfileMainView
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileMainView::CProfileMainView(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler )
    :   iEngineHandler( aEngineHandler ),
        iIndexHandler( aIndexHandler ),
	    iTopItemIndex( KErrNotFound )
	{
	}

// -----------------------------------------------------------------------------
// CProfileMainView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileMainView::ConstructL()
    {
    BaseConstructL( R_PROFILE_MAIN_VIEW );

    CAknTitlePane* titlePane = static_cast< CAknTitlePane* >(
        StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    HBufC* titleText = StringLoader::LoadLC( R_PROFILE_TEXT_TITLE );
    titlePane->SetText( titleText ); // takes ownership
    CleanupStack::Pop( titleText );

    iNotifier = CProfileChangeNotifyHandler::NewL( this );
    
    iLocalFeatures = &( iEngineHandler.Engine()->LocalFeatures() );
    }

// -----------------------------------------------------------------------------
// CProfileMainView::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileMainView* CProfileMainView::NewLC(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler )
    {
    CProfileMainView* self =
        new( ELeave ) CProfileMainView( aEngineHandler, aIndexHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CProfileMainView::~CProfileMainView()
    {
    delete iNotifier;
    delete iNaviPaneContainer;
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainView::DisplayConfirmationQueryL
// -----------------------------------------------------------------------------
//
TBool CProfileMainView::DisplayConfirmationQueryL( TInt aResourceId ) const
	{
	CAknQueryDialog* dlg = CAknQueryDialog::NewL();
	return dlg->ExecuteLD( aResourceId );
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DoCmdActivateL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoCmdActivateL()
	{
	// ID of the currently focused profile
	// (the profile that user would like to activate)
	TInt currentProfileId(
		iEngineHandler.IdForIndex( iIndexHandler.CurrentProfileIndex() ) );

	// If Off-line profile is supported
	if( ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) ) &&
		// and active profile is Off-line
		( iEngineHandler.Engine()->ActiveProfileId() == EProfileOffLineId ) &&
		// and currently focused profile is not Off-line
		( currentProfileId != EProfileOffLineId ) )
		{
		// We are changing from Off-line to a normal profile.
        // Check if SIM card exists or not
#ifndef RD_STARTUP_CHANGE         
        TInt simCStatus( EPSCSimRemoved );
#else
	    TInt simCStatus( ESimNotPresent );
#endif        

		RProperty simStatus;
#ifndef RD_STARTUP_CHANGE
		User::LeaveIfError(
        simStatus.Attach( KUidSystemCategory, KPSUidSimCStatusValue ) );
    	User::LeaveIfError( simStatus.Get( simCStatus ) );
#else
		User::LeaveIfError(
        simStatus.Attach( KPSUidStartup, KPSSimStatus ) );
    	User::LeaveIfError( simStatus.Get( simCStatus ) );
#endif
    	simStatus.Close();

#ifndef RD_STARTUP_CHANGE 		
		if( simCStatus == EPSCSimRemoved )
#else
		if( simCStatus == ESimNotPresent )
#endif		
			{
			// SIM card does not exist.
			// Show an information note and exit from method.
			HBufC* infoText = StringLoader::LoadLC( R_PROFILE_TEXT_INSERT_SIM );
			CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
			note->ExecuteLD( *infoText );
			CleanupStack::PopAndDestroy( infoText );
			return;
			}
		}
	iEngineHandler.Engine()->SetActiveProfileL( currentProfileId );
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DoCmdCreateNewL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoCmdCreateNewL()
	{
	MProfileExtended* profile = iEngineHandler.Engine()->CreateProfileL(); // CSI: 35 # profile object deleted before leaving function calls
	if( profile )
		{
		// Profile creation successful
		TInt id( profile->ProfileName().Id() );
		profile->Release();
		iEngineHandler.ReadIdArrayL();
		// Focus the created profile:
		iIndexHandler.SetCurrentProfileIndex(
			iEngineHandler.IdArray()->FindById( id ) );
		AppUi()->HandleCommandL( EProfileCmdPersonalise );
		}
	else
		{
		// Maximum number of user created profiles reached, show error note
		HBufC* errorText = StringLoader::LoadLC(
			R_PROFILE_TEXT_ERROR_MAXIMUM );
		CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
		note->ExecuteLD( *errorText );
		CleanupStack::PopAndDestroy( errorText );
		}
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DoCmdDeleteL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoCmdDeleteL()
	{
	// Ask user if he/she wants to delete profile
	if( !DisplayConfirmationQueryL( R_PROFILE_DELETE_CONFIRMATION_QUERY ) )
		{
		// User didn't want to delete the profile, return
		return;
		}

    TInt currentProfileIndex( iIndexHandler.CurrentProfileIndex() );

	// Try deleting the focused profile
	TInt errorCode( iEngineHandler.Engine()->DeleteProfileL(
		iEngineHandler.IdForIndex( currentProfileIndex ) ) );

	if( errorCode == KErrNone )
		{
		iEngineHandler.ReadIdArrayL();
		TInt lastIndex = iEngineHandler.IdArray()->MdcaCount() - 1;
		if( currentProfileIndex > lastIndex )
			{
			// The last profile in the list was deleted. change focus.
			iIndexHandler.SetCurrentProfileIndex( lastIndex );
			}
		iContainer->PopulateListBoxL();
		}
	else
		{
		// Show error note if cannot delete profile
		HBufC* errorText = StringLoader::LoadLC(
            R_PROFILE_TEXT_DELETE_ERROR );
		CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
		note->ExecuteLD( *errorText );
		CleanupStack::PopAndDestroy( errorText );
		}
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DoCmdContextSpecificOptionsMenuL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoCmdContextSpecificOptionsMenuL()
	{
	// Switch to Context specific options menu,
	// show it and switch back to main view options menu.
	CEikMenuBar* menuBar = MenuBar();
	menuBar->SetMenuType( CEikMenuBar::EMenuContext );
	menuBar->SetMenuTitleResourceId(
		R_PROFILE_MAIN_VIEW_CONTEXT_SPECIFIC_OPTIONS_MENUBAR );
	
	// TRAP displaying of menu bar.
	// If it fails, the correct resource is set back before leave.
	TRAPD( error, menuBar->TryDisplayContextMenuBarL() );

	menuBar->SetMenuType( CEikMenuBar::EMenuOptions );
	menuBar->SetMenuTitleResourceId( R_PROFILE_MAIN_VIEW_OPTIONS_MENUBAR );
	User::LeaveIfError( error );
	
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DisplayTimedNote
// -----------------------------------------------------------------------------
//	
void CProfileMainView::DisplayTimedInfoNoteL( TInt aMinutes, TInt aHours ) const
	{
	TInt resourceId = R_PROFILE_CONF_SINGULAR;
	TBool minutesSingular( EFalse );
	TBool hoursSingular( EFalse );
		
	if( aMinutes == 1)
		{
		minutesSingular = ETrue;
		}
	if( aHours == 1 )	
		{
		hoursSingular = ETrue;
		}

	if( ( !minutesSingular ) && ( hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_HOUR_SEV_MIN;
		}
	else if( ( minutesSingular ) && ( !hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_SEV_HOURS_MIN;
		}
	else if( ( !minutesSingular ) && ( !hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_PLURAL;
		}
	
	CArrayFix<TInt>* array = new( ELeave ) CArrayFixFlat<TInt>(1);
    CleanupStack::PushL( array );
   
    array->AppendL( aHours );
    array->AppendL( aMinutes );
    	
    HBufC* expiryText = StringLoader::LoadLC(
    	resourceId, *array );
        		
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );

    note->ExecuteLD( *expiryText );

	// expiryText
	CleanupStack::PopAndDestroy( expiryText );
		
	// array
    CleanupStack::PopAndDestroy( array );
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DisplayTimedQueryNote
// -----------------------------------------------------------------------------
//
void CProfileMainView::DisplayTimedQueryNoteL( TInt aMinutes, TInt aHours ) const
	{
	TInt resourceId = R_PROFILE_CONF_SINGULAR;
	TBool minutesSingular( EFalse );
	TBool hoursSingular( EFalse );
		
	if( aMinutes == 1)
		{
		minutesSingular = ETrue;
		}
	if( aHours == 1 )	
		{
		hoursSingular = ETrue;
		}

	if( ( !minutesSingular ) && ( hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_HOUR_SEV_MIN;
		}
	else if( ( minutesSingular ) && ( !hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_SEV_HOURS_MIN;
		}
	else if( ( !minutesSingular ) && ( !hoursSingular ) )
		{
		resourceId = R_PROFILE_CONF_PLURAL;
		}
	
	CArrayFix<TInt>* array = new( ELeave ) CArrayFixFlat<TInt>(1);
    CleanupStack::PushL( array );
   
    array->AppendL( aHours );
    array->AppendL( aMinutes );
    	
    HBufC* expiryText = StringLoader::LoadLC(
    	resourceId, *array );
        		
    CAknQueryDialog* query = CAknQueryDialog::NewL();
    const TInt ret(
     query->ExecuteLD( R_PROFILE_CONFIRM_TIMED_QUERY, *expiryText ) );

	// expiryText
	CleanupStack::PopAndDestroy( expiryText );
		
	// array
    CleanupStack::PopAndDestroy( array );
	}

// -----------------------------------------------------------------------------
// CProfileMainView::DoCmdTimedL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoCmdTimedL()
	{
	// ID of the currently focused profile
	// (the profile that user would like to activate)
	TInt currentProfileId(
		iEngineHandler.IdForIndex( iIndexHandler.CurrentProfileIndex() ) );

	// If Off-line profile is supported
	if( ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) ) &&
		// and active profile is Off-line
		( iEngineHandler.Engine()->ActiveProfileId() == EProfileOffLineId ) &&
		// and currently focused profile is not Off-line
		( currentProfileId != EProfileOffLineId ) )
		{
		// We are changing from Off-line to a normal profile.
        // Check if SIM card exists or not
#ifndef RD_STARTUP_CHANGE         
        TInt simCStatus( EPSCSimRemoved );
#else
	    TInt simCStatus( ESimNotPresent );
#endif        

		RProperty simStatus;
#ifndef RD_STARTUP_CHANGE
		User::LeaveIfError(
        simStatus.Attach( KUidSystemCategory, KPSUidSimCStatusValue ) );
    	User::LeaveIfError( simStatus.Get( simCStatus ) );
#else
		User::LeaveIfError(
        simStatus.Attach( KPSUidStartup, KPSSimStatus ) );
    	User::LeaveIfError( simStatus.Get( simCStatus ) );
#endif
    	simStatus.Close();

#ifndef RD_STARTUP_CHANGE 		
		if( simCStatus == EPSCSimRemoved )
#else
		if( simCStatus == ESimNotPresent )
#endif		
			{
			// SIM card does not exist.
			// Show an information note and exit from method.
			HBufC* infoText = StringLoader::LoadLC( R_PROFILE_TEXT_INSERT_SIM );
			CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
			note->ExecuteLD( *infoText );
			CleanupStack::PopAndDestroy( infoText );
			return;
			}
		}

	TTime expireTime;
	expireTime.HomeTime();
	
	HBufC* expiryTime =
    	StringLoader::LoadLC( R_PROFILE_SETTING_EXPIRY_TIME );
    	 		
	CAknTimeQueryDialog* dlg = new(ELeave) CAknTimeQueryDialog( expireTime );
    if (dlg->ExecuteLD(R_PROFILE_TIME_QUERY, *expiryTime ))
    	{
    	// Timing currently active profile is not allowed. This can happen
    	// if active profile is changed after the timing process begins.
    	// Exception: re-timing a timed profile.
	    MProfileEngine* profileEngine = CreateProfileEngineL();
	    CleanupReleasePushL( *profileEngine );
	    TBool isActiveProfileTimed = profileEngine->IsActiveProfileTimedL();
	    CleanupStack::PopAndDestroy( profileEngine );

    	if( currentProfileId == iEngineHandler.Engine()->ActiveProfileId()
    		 &&	!isActiveProfileTimed )
    		{
			HBufC* errorText = StringLoader::LoadLC(
	            R_PROFILE_ERROR_TIMING_ACTIVE_PROFILE );
			CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
			note->ExecuteLD( *errorText );
			CleanupStack::PopAndDestroy( errorText );
            CleanupStack::PopAndDestroy( expiryTime );
    		return;
    		}
    	
    	TTime originalTime;
        originalTime.HomeTime();

    	TDateTime expiredtime = expireTime.DateTime();
        TDateTime originaltdtime = originalTime.DateTime();
        expiredtime.SetDay(originaltdtime.Day());
        expiredtime.SetMonth(originaltdtime.Month());
        expiredtime.SetYear(originaltdtime.Year());
    	expireTime = expiredtime;
    	
    	if( expireTime < originalTime )
    		{
    		expireTime += TTimeIntervalDays(1);
    		}
		
		TTimeIntervalMicroSeconds remainTime(
		 expireTime.MicroSecondsFrom( originalTime ) );
    	TInt64 tmp = ( remainTime.Int64() ) / KOneMinuteInMicrosecond;
    	TInt minutes = I64INT(tmp);
		TInt hours(minutes / KOneHourInMinute);
    	minutes -= hours * KOneHourInMinute;
	
		DisplayTimedQueryNoteL( minutes, hours );
		
		MProfileEngineExtended* engine = iEngineHandler.Engine();
		engine->SetActiveProfileTimedL( currentProfileId, expireTime );
        }
	CleanupStack::PopAndDestroy( expiryTime );        
	}

// -----------------------------------------------------------------------------
// CProfileMainView::Id
// -----------------------------------------------------------------------------
//
TUid CProfileMainView::Id() const
    {
    return KProfileAppMainViewId;
    }

// -----------------------------------------------------------------------------
// CProfileMainView::HandleCommandL
// -----------------------------------------------------------------------------
//
void CProfileMainView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EProfileCmdActivate:
            {
			DoCmdActivateL();
            break;
            }
		case EProfileCmdCreateNew:
			{
			DoCmdCreateNewL();
			break;
			}
		case EProfileCmdDelete:
			{
			DoCmdDeleteL();
			break;
			}
        case EProfileCmdContextSpecificOptionsMenu:
        case EAknSoftkeyContextOptions:
            {
			DoCmdContextSpecificOptionsMenuL();
            break;
            }
        case EProfileCmdTimed:
        	{
        	DoCmdTimedL();
        	break;
        	}
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainView::DoActivateL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoActivateL(
    const TVwsViewId& /* aPrevViewId */,
    TUid aCustomMessageId,
    const TDesC8& /* aCustomMessage */ )
    {
    if( !iContainer )
        {
        iContainer = CProfileMainContainer::NewL(
            iEngineHandler, iIndexHandler, *this );
		iContainer->PopulateListBoxL( iTopItemIndex );
        iNaviPaneContainer =
            CProfileNaviPaneContainer::NewL( *StatusPane(), iEngineHandler );
        // Set the name of the active profile to navi pane
        iNaviPaneContainer->SetNaviPaneTextL();
        AppUi()->AddToViewStackL( *this, iContainer );
        iContainer->MakeVisible( ETrue );
        }

    if( aCustomMessageId == KProfileAppMainViewId )
    	{
		DoCmdCreateNewL(); 
    	}

    CEikMenuBar* menuBar = MenuBar();
	menuBar->SetContextMenuTitleResourceId(
		R_PROFILE_MAIN_VIEW_CONTEXT_SPECIFIC_OPTIONS_MENUBAR );
		
		
    if ( iEikonEnv->StartedAsServerApp() )
        {
        UpdateCbaL(R_PROFILE_MAIN_VIEW_CBA_2);    
        }
    
    }

// -----------------------------------------------------------------------------
// CProfileMainView::DoDeactivate
// -----------------------------------------------------------------------------
//
void CProfileMainView::DoDeactivate()
    {
    delete iNaviPaneContainer;
    iNaviPaneContainer = NULL;
    if( iContainer )
        {
		iTopItemIndex = iContainer->TopItemIndex();
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CProfileMainView::DynInitMenuPaneL(
    TInt aResourceId, CEikMenuPane* aMenuPane )
	{
	if( !iContainer ||
        ( (aResourceId != R_PROFILE_MAIN_VIEW_OPTIONS_MENUPANE) &&
          (aResourceId != R_PROFILE_MAIN_VIEW_CONTEXT_SPECIFIC_OPTIONS_MENUPANE)
            ) )
        {
        return;
        }

    // ID of the currently focused profile
    TInt currentProfileId(
        iEngineHandler.IdForIndex( iIndexHandler.CurrentProfileIndex() ) );
    TInt activeProfileId( iEngineHandler.Engine()->ActiveProfileId() );

    // "Timed" item is in both menus:
    if( aResourceId == R_PROFILE_MAIN_VIEW_CONTEXT_SPECIFIC_OPTIONS_MENUPANE && (( !iLocalFeatures->IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
            || ( currentProfileId == EProfileOffLineId ) ||
            ( ( activeProfileId == currentProfileId ) && 
              !iEngineHandler.Engine()->IsActiveProfileTimedL() ) ))
        {
        // Timed profiles is not supported. Hide Timed option from
        // Options menu
        aMenuPane->SetItemDimmed( EProfileCmdTimed, ETrue );
        }
    if(aResourceId == R_PROFILE_MAIN_VIEW_CONTEXT_SPECIFIC_OPTIONS_MENUPANE &&  ( FeatureManager::FeatureSupported(KFeatureIdDynamicProfiles) ) &&
                  iEngineHandler.Engine()->IsDefaultProfile( currentProfileId ) )
                {
                // A default profile is focused. Hide Delete from Options menu
                aMenuPane->SetItemDimmed( EProfileCmdDelete, ETrue );
                }
    
    if( aResourceId == R_PROFILE_MAIN_VIEW_OPTIONS_MENUPANE )
        {
        if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            // Help is not supported. Hide Help from Options menu
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        // Check that dynamic profiles are supported
        // before trying to hide Delete from Options menu.
       /* if( ( FeatureManager::FeatureSupported(KFeatureIdDynamicProfiles) ) &&
              iEngineHandler.Engine()->IsDefaultProfile( currentProfileId ) )
            {
            // A default profile is focused. Hide Delete from Options menu
            aMenuPane->SetItemDimmed( EProfileCmdDelete, ETrue );
            }*/
        }

	}

// -----------------------------------------------------------------------------
// CProfileMainView::HandleActiveProfileEventL
// -----------------------------------------------------------------------------
//
void CProfileMainView::HandleActiveProfileEventL(
    TProfileEvent /* aProfileEvent */, TInt /* aProfileId */ )
    {
    if( iNaviPaneContainer )
        {
        // Active profile has changed. Change active profile name in navi pane.
        iNaviPaneContainer->SetNaviPaneTextL();
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainView::UpdateClientRect
// -----------------------------------------------------------------------------
//
void CProfileMainView::UpdateClientRect()
	{
	iContainer->SetRect( ClientRect() );
	}

// -----------------------------------------------------------------------------
// CProfileMainView::View
// -----------------------------------------------------------------------------
//
CAknView& CProfileMainView::View()
    {
    return *this;
    }


// -----------------------------------------------------------------------------
// CProfileMainView::UpdateCbaL
// 
// -----------------------------------------------------------------------------
//
void CProfileMainView::UpdateCbaL(TInt aResourceId)
    {
    CEikButtonGroupContainer* cba = Cba();
    cba->SetCommandSetL(aResourceId);
    cba->DrawDeferred();
    }


// End of File
