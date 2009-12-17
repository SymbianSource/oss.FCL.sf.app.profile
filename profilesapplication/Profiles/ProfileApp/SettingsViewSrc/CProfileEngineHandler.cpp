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
* Description:  Profile engine handler
*
*/



// INCLUDE FILES
#include "CProfileEngineHandler.h"

#include <mprofileengineextended.h>
#include <mprofilesnamesarray.h>
#include <mprofilename.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileEngineHandler::CProfileEngineHandler
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileEngineHandler::CProfileEngineHandler()
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileEngineHandler::ConstructL()
    {
    // Create the Profile Engine
    iEngine = CreateProfileEngineExtendedL();
    // Get a list of profile Id:s
    ReadIdArrayL();
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileEngineHandler* CProfileEngineHandler::NewL()
    {
    CProfileEngineHandler* self = new( ELeave ) CProfileEngineHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor.
CProfileEngineHandler::~CProfileEngineHandler()
    {
    delete iIdArray;
    if( iEngine )
        {
        iEngine->Release();
        }
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::Engine
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngineExtended* CProfileEngineHandler::Engine() const
    {
    return iEngine;
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::ReadIdArrayL
// -----------------------------------------------------------------------------
//
EXPORT_C void CProfileEngineHandler::ReadIdArrayL()
    {
    MProfilesNamesArray* idArray = iEngine->ProfilesNamesArrayLC();
    CleanupStack::Pop();
    delete iIdArray;
    iIdArray = idArray;
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::IdArray
// -----------------------------------------------------------------------------
//
EXPORT_C MProfilesNamesArray* CProfileEngineHandler::IdArray() const
    {
    return iIdArray;
    }

// -----------------------------------------------------------------------------
// CProfileEngineHandler::IdForIndex
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileEngineHandler::IdForIndex( TInt aIndex ) const
    {
    return iIdArray->ProfileName( aIndex )->Id();
    }

//  End of File
