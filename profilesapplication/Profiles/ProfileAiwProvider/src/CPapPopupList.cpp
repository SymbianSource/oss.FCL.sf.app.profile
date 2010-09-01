/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A thin CAknPopupList extension.
*
*/



// CLASS HEADER
#include "CPapPopupList.h"

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <AknIconArray.h>
#include <aknlists.h>
#include <AknsSkinInstance.h>
#include <AknsUtils.h>
#include <aknconsts.h>
#include <avkon.mbg>
#include <gulicon.h>

// CONSTANTS
namespace
	{
	// Granularities
	const TInt KIconGranularity( 3 );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPapPopupList::CPapPopupList
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPapPopupList::CPapPopupList()
	: CAknPopupList()
    {
    }


void CPapPopupList::ConstructL( CEikListBox* aListBox, TInt aCbaResource )
	{
    CAknPopupList::ConstructL( aListBox, aCbaResource, AknPopupLayouts::EMenuWindow );
	}

// -----------------------------------------------------------------------------
// CPapPopupList::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPapPopupList* CPapPopupList::NewL( CEikListBox* aListBox, TInt aCbaResource )
	{
	CPapPopupList* self = new( ELeave ) CPapPopupList();
	CleanupStack::PushL( self );
	self->ConstructL( aListBox, aCbaResource );
	CleanupStack::Pop( self );
	return self;
	}

// Destructor
 CPapPopupList::~CPapPopupList()
    {
    }
    
// -----------------------------------------------------------------------------
// CPapPopupList::HandleResourceChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPapPopupList::HandleResourceChange( TInt aType )
    {
    if( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE( SetIconsL() );
        }
    CAknPopupList::HandleResourceChange( aType );
    }

// -----------------------------------------------------------------------------
// CPapPopupList::HandleListBoxEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPapPopupList::HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType )
    {
    switch( aEventType )
        {
        case EEventItemClicked:
        case EEventItemSingleClicked:
            UpdateCba();
            break;

        default:
            break;
        }

    CAknPopupList::HandleListBoxEventL( aListBox, aEventType );
    }

// -----------------------------------------------------------------------------
// CPapPopupList::ProcessCommandL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPapPopupList::ProcessCommandL( TInt aCommandId )
    {
    switch( aCommandId )
        {
        // Fall through
        case EAknSoftkeyMark:
        case EAknSoftkeyUnmark:
            UpdateCba();
            break;
            
        default:
            break;
        }

    CAknPopupList::ProcessCommandL( aCommandId );
    }

// -----------------------------------------------------------------------------
// CPapPopupList::UpdateCba
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPapPopupList::UpdateCba()
    {
    // Cache the pointer to button group container
    CEikButtonGroupContainer* cba = ButtonGroupContainer();
    // Left softkey should be visible if there are items selected,
    // i.e. the selection index array has items.
    TBool visible( ListBox()->View()->SelectionIndexes()->Count() > 0 );
    // Resolve left softkey command id
    TInt leftId( cba->ButtonGroup()->CommandId( 0 ) );
    // Check if left softkey visibility has changed
    if( visible != cba->IsCommandVisible( leftId ) )
        {
        // Left softkey visibility has changed, update it
        cba->MakeCommandVisible( leftId, visible );
        }
    }

// ----------------------------------------------------------------------------
// CPapPopupList::SetIconsL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CPapPopupList::SetIconsL()
    {
    CAknSingleGraphicPopupMenuStyleListBox* listBox = 
    	static_cast<CAknSingleGraphicPopupMenuStyleListBox*>( ListBox() );

    // Create new icon array
    CAknIconArray* iconArray = new( ELeave ) CAknIconArray( KIconGranularity );
    CleanupStack::PushL( iconArray );

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap = 0;
    CFbsBitmap* mask = 0;
    AknsUtils::CreateColorIconL( skin, KAknsIIDQgnIndiCheckboxOn,
    	KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG15,
    	bitmap, mask, KAvkonBitmapFile, EMbmAvkonQgn_indi_checkbox_on,
    	EMbmAvkonQgn_indi_checkbox_on_mask, KRgbBlack  );
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    icon = CGulIcon::NewLC();
    AknsUtils::CreateColorIconL( skin, KAknsIIDQgnIndiCheckboxOff,
    	KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG15,
    	bitmap, mask, KAvkonBitmapFile, EMbmAvkonQgn_indi_checkbox_off,
    	EMbmAvkonQgn_indi_checkbox_off_mask, KRgbBlack  );
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    // Delete old icons
    CArrayPtr<CGulIcon>* arr = listBox->ItemDrawer()->ColumnData()->IconArray();
    if( arr )
    	{
    	arr->ResetAndDestroy();
	    delete arr;
	    arr = NULL;
    	}

	// Set new icon array
    listBox->ItemDrawer()->ColumnData()->SetIconArray( iconArray );	// changes ownership
    CleanupStack::Pop( iconArray );

    }


//  End of File
