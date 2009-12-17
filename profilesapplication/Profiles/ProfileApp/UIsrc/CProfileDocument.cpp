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
* Description:  The document class for Profiles application.
*
*/


// INCLUDE FILES
#include "CProfileDocument.h"

#include <featmgr.h>
#include <CProfileEngineHandler.h>
#include <CProfileIndexHandler.h>

#include "CProfileAppUi.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileDocument::CProfileDocument
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileDocument::CProfileDocument( CEikApplication& aApp )
	:	CAknDocument( aApp )
	{
    }

// -----------------------------------------------------------------------------
// CProfileDocument::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileDocument::ConstructL()
	{
	FeatureManager::InitializeLibL();
    iEngineHandler = CProfileEngineHandler::NewL();
    iIndexHandler = CProfileIndexHandler::NewL( *iEngineHandler );
	}

// -----------------------------------------------------------------------------
// CProfileDocument::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileDocument* CProfileDocument::NewL( CEikApplication& aApp )
	{
	CProfileDocument* self = new( ELeave ) CProfileDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// Destructor
CProfileDocument::~CProfileDocument()
    {
    delete iIndexHandler;
	delete iEngineHandler;
	FeatureManager::UnInitializeLib();
	}

// -----------------------------------------------------------------------------
// CProfileDocument::CreateAppUiL
// -----------------------------------------------------------------------------
//
CEikAppUi* CProfileDocument::CreateAppUiL()
    {
    return new( ELeave ) CProfileAppUi( *iEngineHandler, *iIndexHandler );
    }

// End of File
