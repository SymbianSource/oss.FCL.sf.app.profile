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
* Description: Settings view class.
*
*/


// INCLUDE FILES
#include    "CProfileSettingsView.h"

#include    <aknViewAppUi.h>
#include    <akntitle.h>
#include    <akntabgrp.h>
#include    <featmgr.h>
#include    <mprofileengineextended.h>
#include    <mprofilesnamesarray.h>
#include    <CProfileEngineHandler.h>
#include    <CProfileIndexHandler.h>
#include    <CProfileTabHandler.h>
#include    <ProfileApp.rsg>

#include    "ProfilePanics.h"
#include    "CProfileSettingsContainer.h"
#include    "ProfileApp.hrh"



/******************************************************************************
 * class CProfileTopControl
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CProfileTopControl::NewL()
// 
// -----------------------------------------------------------------------------
//
CProfileTopControl* CProfileTopControl::NewL( CCoeControl* aFocusControl )
    {
    CProfileTopControl* self = new ( ELeave ) CProfileTopControl();
    self->iFocusControl = aFocusControl;
    return self;
    }


// -----------------------------------------------------------------------------
// CProfileTopControl::FocusChanged()
// 
// -----------------------------------------------------------------------------
//
void CProfileTopControl::FocusChanged( TDrawNow aDrawNow )
    {
    // Not calling super::FocusChanged is intentional
    if( iFocusControl )
        {
        iFocusControl->SetFocus( IsFocused(), aDrawNow );
        }
    }


/******************************************************************************
 * class CProfileDummyHandler
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CProfileDummyHandler::CProfileDummyHandler()
// 
// -----------------------------------------------------------------------------
//
CProfileDummyHandler::CProfileDummyHandler()
    {
    }


// -----------------------------------------------------------------------------
// CProfileDummyHandler::~CProfileDummyHandler()
// 
// -----------------------------------------------------------------------------
//
CProfileDummyHandler::~CProfileDummyHandler()
    {
    }


// -----------------------------------------------------------------------------
// CProfileDummyHandler::CreateSettingItemL()
// 
// This should never be called in Profiles App.
// -----------------------------------------------------------------------------
//
CAknSettingItem* CProfileDummyHandler::CreateSettingItemL( TInt /* aIdentifier */ )
    {
    User::Panic( KProfilePanicText, EProfilePanicInvalidSettingItemId );
    return NULL; // to make compiler happy
    }


// -----------------------------------------------------------------------------
// CProfileDummyHandler::SetObserver()
// 
// -----------------------------------------------------------------------------
//
void CProfileDummyHandler::SetObserver(MProfileExternalSettingsObserver& /* aObserver */ )
   {
   }


// -----------------------------------------------------------------------------
// CProfileDummyHandler::AllowedToChange()
// 
// -----------------------------------------------------------------------------
//
TBool CProfileDummyHandler::AllowedToChange( TInt /* aSettingItemId */ ) const
    {
    return EFalse;
    }



/******************************************************************************
 * class CProfileSettingsView
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CProfileSettingsView::CProfileSettingsView
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileSettingsView::CProfileSettingsView(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler )
    :   iEngineHandler( aEngineHandler ),
        iIndexHandler( aIndexHandler )
    {
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CProfileSettingsView::ConstructL()
    {
    BaseConstructL( R_PROFILE_SETTINGS_VIEW );
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileSettingsView* CProfileSettingsView::NewLC(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler )
    {
    CProfileSettingsView* self = new( ELeave ) CProfileSettingsView(
        aEngineHandler, aIndexHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor.
CProfileSettingsView::~CProfileSettingsView()
    {
    if( iTopControl )
        {
        AppUi()->RemoveFromStack( iTopControl );
        }
    if( iTabHandler )
        {
        AppUi()->RemoveFromStack( iTabHandler->TabGroup() );
        }
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    delete iTabHandler;
    delete iTopControl;
    delete iDummyHandler;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::Id
// -----------------------------------------------------------------------------
//
TUid CProfileSettingsView::Id() const
    {
    return KProfileAppSettingViewId;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::HandleCommandL
// -----------------------------------------------------------------------------
//
void CProfileSettingsView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            if( iContainer && iContainer->ItemEdited() )
                {
                break;
                }
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        case EAknSoftkeyChange: // From context specific options menu
        case EProfileCmdChange:
            {
            if( !iContainer )
                {
                break;
                }
            iContainer->EditItemL(
                iContainer->SettingItemArray()->ItemIndexFromVisibleIndex(
                    iContainer->ListBox()->CurrentItemIndex() ),
                    ( aCommand == EProfileCmdChange ) );
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
// CProfileSettingsView::DoActivateL
// -----------------------------------------------------------------------------
//
void CProfileSettingsView::DoActivateL(
    const TVwsViewId& /* aPrevViewId */,
    TUid aCustomMessageId,
    const TDesC8& aCustomMessage )
    {
    if ( !iContainer )
        {
        // If aCustomMessageId is the same as CProfileSettingsView id,
        // the caller is external. Check if profile id is given and open it.
        if( aCustomMessageId == KProfileAppSettingViewId )
		    {
		    TLex8 lex( aCustomMessage );
		    TInt value = 0;
		    lex.Val( value );
		    TInt err = KErrNone;
		    MProfilesNamesArray* profileNamesArray = iEngineHandler.Engine()->ProfilesNamesArrayLC();
		    TInt index = profileNamesArray->FindById(value);
		    CleanupStack::PopAndDestroy();
		    TRAP( err, iIndexHandler.SetCurrentProfileIndex( index ) );	

		    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
		    CleanupStack::PushL( cba );
		    cba->SetCommandSetL( R_PROFILE_SETTINGS_VIEW_CBA_IDLE );
		    CleanupStack::Pop( cba );
		    cba->DrawDeferred();	
		    }

        // Get pointer to navi pane
        CAknNavigationControlContainer* naviPane =
            static_cast<CAknNavigationControlContainer*>(
		    StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        // Get pointer to title pane
       	CAknTitlePane* titlePane = static_cast< CAknTitlePane* >(
		    StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

        //iTabHandler = CProfileTabHandler::NewL(
        //    iEngineHandler, iIndexHandler, *naviPane );

        iDummyHandler = new ( ELeave ) CProfileDummyHandler();
        iContainer = CProfileSettingsContainer::NewL(
            *this, iEngineHandler, *iDummyHandler, &iIndexHandler,
            iTabHandler, titlePane );

        iTopControl = CProfileTopControl::NewL( iContainer );
  
  		AppUi()->AddToViewStackL( *this, iContainer );
   		// Tab group must be added to stack last.
   		//AppUi()->AddToStackL( *this, iTabHandler->TabGroup() );
   		// Top control is always on top as it listens to focus changes (which
   		// will be routed to container to adapt highlight animation to focus)
   		AppUi()->AddToStackL( *this, iTopControl );

   		iContainer->MakeVisible( ETrue );
        }
   }

// -----------------------------------------------------------------------------
// CProfileSettingsView::DoDeactivate
// -----------------------------------------------------------------------------
//
void CProfileSettingsView::DoDeactivate()
    {
    if( iTopControl )
        {
        AppUi()->RemoveFromStack( iTopControl );
        delete iTopControl;
        iTopControl = NULL;
        }
    if( iTabHandler )
        {
        AppUi()->RemoveFromStack( iTabHandler->TabGroup() );
        delete iTabHandler;
        iTabHandler = NULL;
        }
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    delete iDummyHandler;
    iDummyHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CProfileSettingsView::DynInitMenuPaneL(
    TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    if( ( iContainer ) && 
        ( aResourceId == R_PROFILE_SETTINGS_VIEW_OPTIONS_MENUPANE ) )
        {
        // Check if the setting item is allowed to be changed
        // If not, hide "Change" from Options menu
        CAknSettingItemArray* array = iContainer->SettingItemArray();
        if( !iContainer->AllowedToChange(
            array->At( array->ItemIndexFromVisibleIndex(
            iContainer->ListBox()->CurrentItemIndex() ) )->Identifier() ) )
            {
            aMenuPane->SetItemDimmed( EProfileCmdChange, ETrue );
            }
        if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) ) // Help
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileSettingsView::UpdateClientRect
// -----------------------------------------------------------------------------
//
void CProfileSettingsView::UpdateClientRect()
	{
    iContainer->SetRect( ClientRect() );
	}

// -----------------------------------------------------------------------------
// CProfileSettingsView::View
// -----------------------------------------------------------------------------
//
CAknView& CProfileSettingsView::View()
    {
    return *this;
    }

// End of File
