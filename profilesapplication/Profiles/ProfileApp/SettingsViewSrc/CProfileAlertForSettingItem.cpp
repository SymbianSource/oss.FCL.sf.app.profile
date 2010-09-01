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
* Description:  Setting item class for "Alert for" setting.
*
*/



// INCLUDES
#include "CProfileAlertForSettingItem.h"

#include <coemain.h>
#include <cntdb.h>
#include <cntitem.h>
#include <StringLoader.h>
#include <CPbkContactEngine.h>
#include <CPbkContactChangeNotifier.h>
#include <profilesettingsview.rsg>

#include "CProfileAlertForSettingPage.h"

// CONSTANTS
const TInt KContactItemArrayGranularity( 8 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::CProfileAlertForSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileAlertForSettingItem::CProfileAlertForSettingItem(
    TInt aIdentifier,
    RArray<TContactItemId>& aContactList,
    CCoeEnv* aCoeEnv )
    :   CAknSettingItem( aIdentifier ),
        iPopupArray( KContactItemArrayGranularity ),
        iExternalContactArray( aContactList ),
        iCoeEnv( aCoeEnv )
    {
    }

// Destructor
CProfileAlertForSettingItem::~CProfileAlertForSettingItem()
    {
    delete iChangeNotifier;
    delete iPbkContactEngine;
    delete iTextSeveralGroups;
    delete iTextAllCallers;
    iFullContactArray.Close();
    iPopupArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::CompleteConstructionL()
    {
    // Read localised texts from resource
    iTextAllCallers = StringLoader::LoadL( R_PROFILE_TEXT_ALL_CALLERS );
	iTextSeveralGroups = StringLoader::LoadL( R_PROFILE_TEXT_SEVERAL_GROUPS );

    // Create Phonebook contact engine and change notifier
    iPbkContactEngine = CPbkContactEngine::NewL();
    iChangeNotifier = iPbkContactEngine->CreateContactChangeNotifierL( this );

    PopulatePopupArrayL();
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::AlertForStatus
// -----------------------------------------------------------------------------
//
CProfileAlertForSettingItem::TAlertForStatus CProfileAlertForSettingItem::AlertForStatus() const
    {
    TInt count( iPopupArray.Count() );
    if( count > 1 )
        {
        // Array contains 2 or more items
        if( SelectionStatus( 0 ) )
            {
            // If first item is selected, return EAlertForAllCallers
            return EAlertForAllCallers;
            }
        TInt selectCount( 0 );
        // Start from index 1, because 0 is "All callers"
        for( TInt index( 1 ); index < count; index++ )
            {
            if( SelectionStatus( index ) )
                {
                // Found a selected item, increase counter
                selectCount++;
                }
            }
        if( selectCount == 1 )
            {
            return EAlertForSingleGroup;
            }
        return EAlertForSeveralGroups;
        }
    // Pop-up array is empty or contains only "All callers"
    return EAlertForAllCallers;
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::SetSelectionStatus
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::SetSelectionStatus( TInt aIndex, TBool aStatus )
    {
    if( aIndex < iPopupArray.MdcaCount()  )
	    {
    	iPopupArray.At( aIndex )->SetSelectionStatus( aStatus );
	    }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::SelectionStatus
// -----------------------------------------------------------------------------
//
TBool CProfileAlertForSettingItem::SelectionStatus( TInt aIndex ) const
    {
    if( aIndex < iPopupArray.MdcaCount()  )
	    {
	    return iPopupArray.At( aIndex )->SelectionStatus();
	    }
	else
		{
		return EFalse;
		}	    
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::PopulatePopupArrayL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::PopulatePopupArrayL()
    {
    iPopupArray.ResetAndDestroy();
    iFullContactArray.Reset();

    // Create first item to popup array, "All callers"
    CSelectableItem* item = new( ELeave ) CSelectableItem(
        *iTextAllCallers, EFalse );
    CleanupStack::PushL( item );
    item->ConstructL();
    iPopupArray.AppendL( item ); // Ownership is transferred
    CleanupStack::Pop( item );

    CContactIdArray* groupIdArray = iPbkContactEngine->Database().GetGroupIdListL();
    CleanupStack::PushL( groupIdArray );
    TInt count( groupIdArray->Count() );
    TContactItemId contactId;
    CContactGroup* contactGroup = NULL;
    TPtrC contactGroupName;
    for( TInt index( 0 ); index < count; index++ )
        {
        contactId = ( *groupIdArray )[ index ];

        iFullContactArray.Append( contactId );
        contactGroup = iPbkContactEngine->ReadContactGroupL( contactId );
        CleanupStack::PushL( contactGroup );
        contactGroupName.Set( contactGroup->GetGroupLabelL() );

        item = new( ELeave ) CSelectableItem( contactGroupName, EFalse );
        CleanupStack::PushL( item );
        item->ConstructL();
        iPopupArray.AppendL( item ); // Ownership is transferred
        CleanupStack::Pop( item ); 
        
        CleanupStack::PopAndDestroy( contactGroup );
        }

    CleanupStack::PopAndDestroy( groupIdArray );
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::CheckAllCallersSetting
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::CheckAllCallersSetting()
    {
    TInt count( iPopupArray.Count() );
    if( count > 0 )
        {
        // Check whether the first item, "All callers", is selected or not.
        if( SelectionStatus( 0 ) )
            {
            // "All callers" is selected.
            // Set other groups unselected.
            for( TInt index( 1 ); index < count; index++ )
                {
                SetSelectionStatus( index, EFalse );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::UpdateSettingPageL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::UpdateSettingPageL()
    {
    // Closing and opening the database has  because
    // the database doesn't update its information otherwise
    iPbkContactEngine->Database().CloseTables();
    iPbkContactEngine->Database().OpenTablesL();

    RArray<TContactItemId> selectedContacts;
    CleanupClosePushL( selectedContacts );
    StoreL( selectedContacts );
    PopulatePopupArrayL();
    LoadL( selectedContacts );
    CleanupStack::PopAndDestroy(); // selectedContacts

    UpdateListBoxTextL(); // Calls SettingTextL()

    if( SettingPage() )
        {
        // Pop-up is popped up
        CProfileAlertForSettingPage* sp =
            static_cast< CProfileAlertForSettingPage* >( SettingPage() );
        TInt lastIndex( iPopupArray.Count() - 1 );
        TInt currentIndex( sp->ListBoxControl()->View()->CurrentItemIndex() );
        if( currentIndex > lastIndex )
            {
            sp->ListBoxControl()->SetCurrentItemIndex( lastIndex );
            }
        if( currentIndex < 0 )
            {
            sp->ListBoxControl()->SetCurrentItemIndex( 0 );
            }

        // Must clear listbox view's selections, because if items have been deleted,
        // non existent selections remain in view's selection array.
        sp->ListBoxControl()->View()->ClearSelection();
        // Set the new selection array to setting page
        sp->SetSelectionItemArrayL( &iPopupArray );
        // Update possibly changed CBA visibility
        sp->UpdateCba();
        }
    else // Setting page is not visible
        {
        LoadL();
        }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::StoreL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::StoreL( RArray<TContactItemId>& aArray ) const
    {
    aArray.Reset();
    TInt count( iPopupArray.Count() );
    // Start from index 1 (0 is 'All callers')
    for( TInt index( 1 ); index < count; index++ )
        {
        // If an item in array is selected
        if( SelectionStatus( index ) )
            {
            // Add its id to external contact array
            aArray.Append( iFullContactArray[ index - 1 ] );
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::LoadL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::LoadL( const RArray<TContactItemId>& aArray )
    {
    TInt index( 0 );
    TInt count( iPopupArray.Count() );

    // Set all selections false
    for( ; index < count; index++ )
        {
        SetSelectionStatus( index, EFalse );
        }

    TContactItemId contactId;
    TInt foundCount( 0 );
    TInt foundIndex;
    count = aArray.Count();
    for( index = 0; index < count; index++ )
        {
        contactId = aArray[ index ];
        // Try to find contact id from full contact group array
        foundIndex = iFullContactArray.Find( contactId );
        if( foundIndex >= 0 )
            {
            // Indices in iFullContactArray are the same as in iPopupArray
            // except that iPopupArray's 1st item is "All callers". Therefore + 1.
            SetSelectionStatus( foundIndex + 1, ETrue );
            // A valid contact id is found, increase counter
            foundCount++;
            }
        }

    // If there were no contact groups selected,
    // or selected groups are not longer valid, select "All callers"
    if( foundCount == 0 )
        {
        SetSelectionStatus( 0, ETrue );
        }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::StoreL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::StoreL()
    {
    StoreL( iExternalContactArray );
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::LoadL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::LoadL()
    {
    LoadL( iExternalContactArray );
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::EditItemL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::EditItemL( TBool /* aCalledFromMenu */ )
    {
    CProfileAlertForSettingPage* dlg = new( ELeave ) CProfileAlertForSettingPage(
        SettingPageResourceId(), &iPopupArray );
    SetSettingPage( dlg ); // Set iSettingPage to indicate that pop-up is displayed
    dlg->SetSettingPageObserver( this );
    dlg->ExecuteLD();
    SetSettingPage( NULL ); // iSettingPage is NULL, pop-up is closed
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::SettingTextL
// -----------------------------------------------------------------------------
//
const TDesC& CProfileAlertForSettingItem::SettingTextL()
    {
    switch( AlertForStatus() )
        {
        case EAlertForAllCallers:
            {
            iSettingText = *iTextAllCallers;
            break;
            }
        case EAlertForSingleGroup:
            {
            TInt count( iPopupArray.Count() );
            if( count > 0 )
                {
                // Search the selected group
                while( !SelectionStatus( --count ) && count > 1 ) {}
                // Get its name
                iSettingText = iPopupArray.At( count )->ItemText();
                }
            break;
            }
        case EAlertForSeveralGroups:
            {
            iSettingText = *iTextSeveralGroups;
            break;
            }
        default:
            {
            break;
            }
        }
    return iSettingText;
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::HandleDatabaseEventL
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::HandleDatabaseEventL( TContactDbObserverEvent aEvent )
    {
    if( ( aEvent.iType == EContactDbObserverEventGroupAdded ) || // A contact item group has been added.
        ( aEvent.iType == EContactDbObserverEventGroupChanged ) || // A contact item group has been updated.
        ( aEvent.iType == EContactDbObserverEventGroupDeleted ) ) // A contact item group has been deleted from the database.
        {
        UpdateSettingPageL();
        }
    }

// -----------------------------------------------------------------------------
// CProfileAlertForSettingItem::HandleSettingPageEventL
// After settings have been accepted, some checks are made in this method.
// If "Alert for" is selected , all other groups must be unselected.
// If nothing is selected , "Alert for" must be selected.
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingItem::HandleSettingPageEventL(
    CAknSettingPage* aSettingPage, TAknSettingPageEvent aEventType )
    {
    switch( aEventType )
        {
        case EEventSettingOked:
            {
            // Check that "All callers" is in correct state
            CheckAllCallersSetting();
            break;
            }
        case EEventSettingCancelled:
            {
            // If setting is cancelled, load old values
            LoadL();
            break;
            }
        case EEventSettingChanged:
            {
            // If setting has changed, update CBA visibility
            static_cast<CProfileAlertForSettingPage*>( aSettingPage )->UpdateCba();
            break;
            }
        default:
            {
            break;
            }
        }
    CAknSettingItem::HandleSettingPageEventL( aSettingPage, aEventType );
    }


//  End of File
