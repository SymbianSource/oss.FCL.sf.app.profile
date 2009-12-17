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
* Description:  Profile index handler (for use of Settings Container
*                if multiple profiles are edited)
*
*/


//  INCLUDES
#include "CProfileIndexHandler.h"

#include <mprofilesnamesarray.h>
#include <eiklbx.h>

#include "CProfileEngineHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileIndexHandler::CProfileIndexHandler
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileIndexHandler::CProfileIndexHandler(
    CProfileEngineHandler& aEngineHandler )
    :   iEngineHandler( aEngineHandler )
    {
    }

void CProfileIndexHandler::ConstructL()
    {
    }

EXPORT_C CProfileIndexHandler* CProfileIndexHandler::NewL(
    CProfileEngineHandler& aEngineHandler )
    {
    CProfileIndexHandler* self =
        new( ELeave ) CProfileIndexHandler( aEngineHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CProfileIndexHandler::~CProfileIndexHandler()
    {
    iIndexArray.Close();
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::CurrentProfileIndex
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileIndexHandler::CurrentProfileIndex() const
	{
	return iCurrentProfileIndex;
	}

// -----------------------------------------------------------------------------
// CProfileIndexHandler::SetCurrentProfileIndex
// Note that there is no boundary checking.
// -----------------------------------------------------------------------------
//
EXPORT_C void CProfileIndexHandler::SetCurrentProfileIndex( TInt aIndex )
	{
	iCurrentProfileIndex = aIndex;
	}

// -----------------------------------------------------------------------------
// CProfileIndexHandler::CreateIndexArrayL
// Resets the index array and populates it with zeroes
// -----------------------------------------------------------------------------
//
void CProfileIndexHandler::CreateIndexArrayL()
    {
    iIndexArray.Reset();
    TItem item = { 0, 0 };
    TInt count( iEngineHandler.IdArray()->MdcaCount() );
    while( --count >= 0 )
        {
        User::LeaveIfError( iIndexArray.Append( item ) );
        }
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::StoreIndices
// -----------------------------------------------------------------------------
//
void CProfileIndexHandler::StoreIndices()
    {
    if( !iListBox )
        {
        return;
        }

    TItem& item = iIndexArray[iCurrentProfileIndex];
    item.iIndex = iListBox->View()->CurrentItemIndex();
    item.iTopIndex = iListBox->View()->TopItemIndex() - item.iIndex;
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::LoadIndices
// -----------------------------------------------------------------------------
//
void CProfileIndexHandler::LoadIndices()
    {
    if( !iListBox )
        {
        return;
        }

    const TInt KMaxLastIndex( 2 ); // Three items fit to screen at the same time

    TInt lastIndex( iListBox->Model()->MatchableTextArray()->MdcaCount() - 1 );

    TItem item = iIndexArray[iCurrentProfileIndex];
    if( item.iIndex > lastIndex )
        {
        item.iIndex = lastIndex;
        }
    if( item.iIndex < 0 )
        {
        item.iIndex = 0;
        }
    item.iTopIndex += item.iIndex;
    if( item.iTopIndex > lastIndex - KMaxLastIndex )
        {
        item.iTopIndex = lastIndex - KMaxLastIndex;
        }
    if( item.iTopIndex < 0 )
        {
        item.iTopIndex = 0;
        }

    iListBox->View()->SetCurrentItemIndex( item.iIndex );
    iListBox->View()->SetTopItemIndex( item.iTopIndex );
    iListBox->DrawDeferred();
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::IndexAt
// -----------------------------------------------------------------------------
//
TInt CProfileIndexHandler::IndexAt( TInt aIndex ) const
    {
    return iIndexArray[aIndex].iIndex;
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::TopIndexAt
// -----------------------------------------------------------------------------
//
TInt CProfileIndexHandler::TopIndexAt( TInt aIndex ) const
    {
    return iIndexArray[aIndex].iTopIndex;
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::RemoveAndInsert
// -----------------------------------------------------------------------------
//
TInt CProfileIndexHandler::RemoveAndInsert(
    TInt aOldIndex, TInt aNewIndex, TInt aIndexValue, TInt aTopIndexValue )
    {
    TInt count( iIndexArray.Count() );
    if( aOldIndex < 0 || aOldIndex >= count )
        {
        return KErrNotFound;
        }
    if( aNewIndex < 0 || aNewIndex > count )
        {
        return KErrArgument;
        }
    iIndexArray.Remove( aOldIndex );
    TItem item = { aIndexValue, aTopIndexValue };
    iIndexArray.Insert( item, aNewIndex );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::SetListBox
// -----------------------------------------------------------------------------
//
void CProfileIndexHandler::SetListBox( CEikListBox* aListBox )
    {
    iListBox = aListBox;
    }

// -----------------------------------------------------------------------------
// CProfileIndexHandler::ReadIdArrayAndUpdateL
// -----------------------------------------------------------------------------
//
TInt CProfileIndexHandler::ReadIdArrayAndUpdateL()
    {
    // Index of the profile before the engine rearranges the profiles.
    TInt oldProfileIndex( iCurrentProfileIndex );
    
    // ID of the profile so that we can find it after the engine
    // rearranges the profiles.
    TInt profileId( iEngineHandler.IdForIndex( oldProfileIndex ) );
    
    // Store the indices for edited profile.
    StoreIndices();
    
    TInt newItemIndex( IndexAt( oldProfileIndex ) );
    TInt newTopItemIndex( TopIndexAt( oldProfileIndex ) );

    // Refresh profile names. Order of the profiles may change.
    iEngineHandler.ReadIdArrayL();

    // Find out which of the profiles is now the edited profile
    TInt newProfileIndex( iEngineHandler.IdArray()->FindById( profileId ) );

    // Set it to current profile
    SetCurrentProfileIndex( newProfileIndex );
    
    // Update indices
    User::LeaveIfError( RemoveAndInsert( oldProfileIndex, newProfileIndex,
        newItemIndex, newTopItemIndex ) );

    return newProfileIndex;
    }

//  End of File
