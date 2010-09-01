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
* Description:  Tab handler for handling tabs in Profiles
*
*/



// CLASS HDEADER
#include "CProfileTabHandler.h"

// INTERNAL INCLUDES
#include "CProfileEngineHandler.h"
#include "CProfileIndexHandler.h"
#include "ProfileEngineConstants.h"

// EXTERNAL INCLUDES
#include <aknnavi.h>
#include <coeaui.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include <AknUtils.h> // AknTextUtils
#include <MProfilesNamesArray.h>

#include <MProfileName.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileTabHandler::CProfileTabHandler
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileTabHandler::CProfileTabHandler(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler,
    CAknNavigationControlContainer& aNaviPane )
    :   iEngineHandler( aEngineHandler ),
        iIndexHandler( aIndexHandler ),
        iNaviPane( aNaviPane )
    {
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::ConstructL()
    {
    CreateTabGroupL();
    iNaviPane.PushL( *iNaviDecorator );
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileTabHandler* CProfileTabHandler::NewL(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler,
    CAknNavigationControlContainer& aNaviPane )
    {
    CProfileTabHandler* self = new( ELeave ) CProfileTabHandler(
        aEngineHandler, aIndexHandler, aNaviPane );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProfileTabHandler::~CProfileTabHandler()
    {
    if( iNaviDecorator )
        {
        iNaviPane.Pop( iNaviDecorator );
        delete iNaviDecorator;
        }
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::TabGroup
// -----------------------------------------------------------------------------
//
EXPORT_C CAknTabGroup* CProfileTabHandler::TabGroup() const
    {
    return iTabGroup;
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::SetActiveTab
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::SetActiveTab( TInt aIndex )
    {
    iTabGroup->SetActiveTabByIndex( aIndex );
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::RefreshTabsL
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::RefreshTabsL()
    {
    MProfilesNamesArray* array = iEngineHandler.IdArray();
    TInt count( array->MdcaCount() );
    if( iTabGroup->TabCount() == count )
        {
        RBuf name;
        name.CreateL( KProfileMaxNameLength );
        CleanupClosePushL( name );
        for( TInt index( 0 ); index < count; index++ )
            {
            name.Copy( array->ProfileName( index )->ShortName() );
            AknTextUtils::DisplayTextLanguageSpecificNumberConversion( name );
            iTabGroup->ReplaceTabTextL( index, name );
            }
        CleanupStack::PopAndDestroy( &name );
        }
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::SetTabObserver
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::SetTabObserver( MAknTabObserver* aTabObserver )
    {
    iTabObserver = aTabObserver;
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::TabChangedL
// CAknTabGroup notifies that tab has changed.
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::TabChangedL( TInt aIndex )
    {
    iIndexHandler.StoreIndices();
    iIndexHandler.SetCurrentProfileIndex( aIndex );
    if( iTabObserver )
        {
        iTabObserver->TabChangedL( aIndex );
        }
    }

// -----------------------------------------------------------------------------
// CProfileTabHandler::CreateTabGroupL
// -----------------------------------------------------------------------------
//
void CProfileTabHandler::CreateTabGroupL()
    {
    // Remove all existing tabs, just in case
    if( iNaviDecorator )
        {
        iNaviPane.Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }

    // Create tab group
    iNaviDecorator = iNaviPane.CreateTabGroupL( this );
    iTabGroup = static_cast<CAknTabGroup*>( iNaviDecorator->DecoratedControl() );

    MProfilesNamesArray* array = iEngineHandler.IdArray();

    RBuf name;
    name.CreateL( KProfileMaxNameLength );
    CleanupClosePushL( name );
    // Add tabs to tab group
    TInt count( array->MdcaCount() );
    for( TInt index( 0 ); index < count; index++ )
        {
        name.Copy( array->ProfileName( index )->ShortName() );
        AknTextUtils::DisplayTextLanguageSpecificNumberConversion( name );
        iTabGroup->AddTabL( index, name );
        }
    CleanupStack::PopAndDestroy( &name );
    
    // Set tab width
    TInt tab( KTabWidthWithThreeLongTabs );
    if( count == 1 )
        {
        tab = KTabWidthWithOneTab;
        }
    else if( count == 2 )
        {
        tab = KTabWidthWithTwoLongTabs;
        }
    iTabGroup->SetTabFixedWidthL( tab );

    // Set active tab
    iTabGroup->SetActiveTabByIndex( iIndexHandler.CurrentProfileIndex() );
    }

//  End of File
