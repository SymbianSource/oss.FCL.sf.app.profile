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
* Description:  Container for navi pane decorator.
*
*/



// INCLUDE FILES
#include    "CProfileNaviPaneContainer.h"

#include    <aknnavide.h>
#include    <aknnavilabel.h>
#include    <eikspane.h>
#include    <avkon.hrh> // EEikStatusPaneUidNavi
#include    <AknUtils.h> // AknTextUtils
#include    <MProfilesNamesArray.h>
#include    <MProfileEngineExtended.h>
#include    <MProfileName.h>

#include    "CProfileEngineHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::CProfileNaviPaneContainer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileNaviPaneContainer::CProfileNaviPaneContainer(
    CProfileEngineHandler& aEngineHandler )
    :   iEngineHandler( aEngineHandler )
    {
    }

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileNaviPaneContainer::ConstructL( CEikStatusPane& aStatusPane )
    {
    iNaviPane = static_cast< CAknNavigationControlContainer* >(
        aStatusPane.ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    iNaviDecorator = iNaviPane->CreateNavigationLabelL();
    iNaviPane->PushL( *iNaviDecorator );
    }

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileNaviPaneContainer* CProfileNaviPaneContainer::NewL(
    CEikStatusPane& aStatusPane, CProfileEngineHandler& aEngineHandler )
    {
    CProfileNaviPaneContainer* self =
        new( ELeave ) CProfileNaviPaneContainer( aEngineHandler );
    CleanupStack::PushL( self );
    self->ConstructL( aStatusPane );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProfileNaviPaneContainer::~CProfileNaviPaneContainer()
    {
   	Release();
    }

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::SetNaviPaneTextL
// -----------------------------------------------------------------------------
//
EXPORT_C void CProfileNaviPaneContainer::SetNaviPaneTextL( const TDesC& aText )
    {
    CAknNaviLabel* label = static_cast< CAknNaviLabel* >(
        iNaviDecorator->DecoratedControl() );
    RBuf title;
    title.CreateL( aText );
    CleanupClosePushL( title );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( title );
    label->SetTextL( title );
    CleanupStack::PopAndDestroy( &title );
    label->DrawDeferred();
    }

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::SetNaviPaneTextL
// -----------------------------------------------------------------------------
//
EXPORT_C void CProfileNaviPaneContainer::SetNaviPaneTextL()
    {
    iEngineHandler.ReadIdArrayL();
    MProfilesNamesArray* nameArray = iEngineHandler.IdArray();
    TInt index( nameArray->FindById( iEngineHandler.Engine()->ActiveProfileId() ) );
    SetNaviPaneTextL( nameArray->ProfileName( index )->NaviName() );
    }

// -----------------------------------------------------------------------------
// CProfileNaviPaneContainer::SetNaviPaneTextL
// -----------------------------------------------------------------------------
//
void CProfileNaviPaneContainer::Release()
    {
  	if( iNaviDecorator )
        {
        iNaviPane->Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }
    }

// End of File
