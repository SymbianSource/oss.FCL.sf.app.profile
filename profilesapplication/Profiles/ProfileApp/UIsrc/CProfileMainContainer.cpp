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
* Description: Container class of main view.
*
*/


// INCLUDE FILES

#include "CProfileMainContainer.h"

#include <AknDef.h>
#include <aknlists.h>
#include <AknUtils.h> // AknTextUtils
#include <featmgr.h>
#include <CProfileEngineHandler.h>
#include <CProfileIndexHandler.h>
#include <mprofileengineextended.h>
#include <mprofilesnamesarray.h>
#include <csxhelp/mode.hlp.hrh>
#include <profileeng.hrh>

#include "CProfileMainView.h"
#include "CProfileDocument.h"
#include "CProfileApplication.h"
#include "ProfileApp.hrh"
#include "profileengineconstants.h"

// CONSTANTS
_LIT( KTabChar, "\t" );
const TInt KProfileNameArrayGranularity( 5 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileMainContainer::CProfileMainContainer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileMainContainer::CProfileMainContainer(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler,
    CAknView& aView )
    :   iEngineHandler( aEngineHandler ),
        iIndexHandler( aIndexHandler ),
        iView( aView )
    {
    SetMopParent( &aView );
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();

    iListBox = new( ELeave ) CAknSingleStyleListBox();
    iListBox->ConstructL( this );
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    SetRect( aRect );
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileMainContainer* CProfileMainContainer::NewL(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler,
    CAknView& aView )
    {
    CProfileMainContainer* self = new( ELeave ) CProfileMainContainer(
        aEngineHandler, aIndexHandler, aView );
    CleanupStack::PushL( self );
    self->ConstructL( aView.ClientRect() );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProfileMainContainer::~CProfileMainContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::PopulateListBoxL
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::PopulateListBoxL( TInt aTopItemIndex )
    {
    // Transfer decorated array ownership for listbox model
    CDesCArrayFlat* decoratedArray =
        new( ELeave ) CDesCArrayFlat( KProfileNameArrayGranularity );
    CTextListBoxModel* model = iListBox->Model();
    model->SetOwnershipType( ELbmOwnsItemArray );
    model->SetItemTextArray( decoratedArray );

    MProfilesNamesArray* array = iEngineHandler.IdArray();

    // Populate the decorated array for listbox
    TInt count( array->MdcaCount() );
    TBuf<PROFILES_MAX_NAME_LENGTH + 1> name; // + 1 for tab character
    for( TInt index( 0 ); index < count; index++ )
        {
        name.Copy( KTabChar );
        TBuf< KProfileMaxNameLength > tmpName( array->MdcaPoint( index ) );
        AknTextUtils::DisplayTextLanguageSpecificNumberConversion( tmpName );
        AknTextUtils::ReplaceCharacters( tmpName, KAknReplaceListControlChars,
                                         TChar( ' ' ) );
        name.Append( tmpName );
        decoratedArray->AppendL( name );
        }

    // Update listbox
    iListBox->HandleItemAdditionL();

    // Update view
    if( aTopItemIndex >= 0 )
        {
        iListBox->SetTopItemIndex( aTopItemIndex );
        }
    iListBox->SetCurrentItemIndex( iIndexHandler.CurrentProfileIndex() );
    iListBox->DrawNow();
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::TopItemIndex
// -----------------------------------------------------------------------------
//
TInt CProfileMainContainer::TopItemIndex() const
    {
    return iListBox->TopItemIndex();
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CProfileMainContainer::CountComponentControls() const
    {
    return KMainContainerSubControlCount;
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CProfileMainContainer::ComponentControl( TInt aIndex ) const
    {
    switch( aIndex )
        {
        case 0:
            {
            return iListBox;
            }
        default:
            {
            return NULL;
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::SizeChanged
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::SizeChanged()
    {
    if( iListBox )
        {
        iListBox->SetRect( Rect() );    // Set container's rect to listbox
        // Force refreshing of iListBox:
        TRAP_IGNORE( iListBox->HandleItemAdditionL() );
        }
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CProfileMainContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }

    switch( aKeyEvent.iCode )
        {
        case EKeyUpArrow: // flow through, both change focus
        case EKeyDownArrow:
            {
            TKeyResponse response( iListBox->OfferKeyEventL( aKeyEvent, aType ) );
            iIndexHandler.SetCurrentProfileIndex( iListBox->CurrentItemIndex() );
            return response;
            }
        case EKeyBackspace: // Clear key
            {
            if( ( FeatureManager::FeatureSupported( KFeatureIdDynamicProfiles ) ) &&
                ( !iEngineHandler.Engine()->IsDefaultProfile(
                    iEngineHandler.IdForIndex( iIndexHandler.CurrentProfileIndex() ) ) ) )
                {
                // If dynamic profiles feature is supported and none of
                // the default profiles is focused, try to delete a profile.
                iView.HandleCommandL( EProfileCmdDelete );
                return EKeyWasConsumed;
                }
            break;
            }
        default:
            {
            break;
            }
        }
    return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::HandleListBoxEventL
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::HandleListBoxEventL(
    CEikListBox* /* aListBox */, TListBoxEvent aEventType )
    {
    if( ( aEventType == EEventEnterKeyPressed ) ||
        
        ( aEventType == EEventItemSingleClicked ) )
    	{
    	iIndexHandler.SetCurrentProfileIndex( iListBox->CurrentItemIndex() );
        iView.HandleCommandL( EProfileCmdContextSpecificOptionsMenu );
    	}
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::GetHelpContext
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidProfileApp;
    aContext.iContext = KMODE_HLP_PROFILES_LIST;
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::HandleResourceChange
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::HandleResourceChange( TInt aType )
    {
    if ( aType == KEikDynamicLayoutVariantSwitch ||
         aType == KAknsMessageSkinChange )
        {
        (static_cast <CProfileMainView *> (&iView))->UpdateClientRect();
        }
        
    CCoeControl::HandleResourceChange( aType );   
    }

// -----------------------------------------------------------------------------
// CProfileMainContainer::FocusChanged
// -----------------------------------------------------------------------------
//
void CProfileMainContainer::FocusChanged( TDrawNow aDrawNow )
    {
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    }

// End of File
