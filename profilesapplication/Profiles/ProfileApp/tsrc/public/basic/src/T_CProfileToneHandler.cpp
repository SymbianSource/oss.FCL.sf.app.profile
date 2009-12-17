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
* Description:  Test class for CProfileIndexHandler.
*
*/



// INCLUDE FILES
#include    "T_CProfileToneHandler.h"
#include    <EUnitMacros.h>
#include    <CProfileToneHandler.h>

namespace
	{
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::T_CProfileToneHandler
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
T_CProfileToneHandler::T_CProfileToneHandler()
    {
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
T_CProfileToneHandler* T_CProfileToneHandler::NewLC()
    {
    T_CProfileToneHandler* self = new( ELeave ) T_CProfileToneHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
T_CProfileToneHandler::~T_CProfileToneHandler()
    {
    }


// -----------------------------------------------------------------------------
// T_CProfileToneHandler::SetupL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::SetupL()
    {
    iToneHandler = CProfileToneHandler::NewL();
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::EmptySetupL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::EmptySetupL()
    {
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::Teardown
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::Teardown()
    {
    delete iToneHandler;
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::NewLTestL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::NewLTestL()
    {
    CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
    CleanupStack::PushL( toneHandler );
    EUNIT_ASSERT( toneHandler );
    CleanupStack::PopAndDestroy(); // toneHandler

    toneHandler = CProfileToneHandler::NewL( );
    CleanupStack::PushL( toneHandler );
    EUNIT_ASSERT( toneHandler );

    CProfileToneHandler* anotherToneHandler = CProfileToneHandler::NewL( );
    CleanupStack::PushL( anotherToneHandler );
    EUNIT_ASSERT( anotherToneHandler );
    CleanupStack::PopAndDestroy(); // anotherToneHandler

    CleanupStack::PopAndDestroy(); // toneHandler
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::NewLCTestL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::NewLCTestL()
    {
    CProfileToneHandler* toneHandler = CProfileToneHandler::NewLC();
    EUNIT_ASSERT( toneHandler );
    CleanupStack::PopAndDestroy(); // toneHandler

    toneHandler = CProfileToneHandler::NewLC( );
    EUNIT_ASSERT( toneHandler );

    CProfileToneHandler* anotherToneHandler = CProfileToneHandler::NewLC( );
    EUNIT_ASSERT( anotherToneHandler );
    CleanupStack::PopAndDestroy(); // anotherToneHandler

    CleanupStack::PopAndDestroy(); // toneHandler
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::SetActiveProfileRingingToneTestL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::SetActiveProfileRingingToneTestL()
    {
    iToneHandler->SetActiveProfileRingingToneL( KNullDesC );
    }

// -----------------------------------------------------------------------------
// T_CProfileToneHandler::SetProfileToneTestL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::SetProfileToneTestL()
    {
    iToneHandler->SetProfileToneL( 0, EProfileRingingToneSetting, KNullDesC );
    }
    
// -----------------------------------------------------------------------------
// T_CProfileToneHandler::SetToneForAllProfilesTestL
// -----------------------------------------------------------------------------
//
void T_CProfileToneHandler::SetToneForAllProfilesTestL()
    {
    iToneHandler->SetToneForAllProfilesL( EProfileRingingToneSetting, KNullDesC );
    }    
    
// ========================== EUNIT TEST CASES =================================

EUNIT_BEGIN_TEST_TABLE(
    T_CProfileToneHandler, // Name of the tester class
    "T_CProfileToneHandler test suite.", // Textual description of the test suite.
    "MODULE" )

EUNIT_TEST(
    "NewLTestL",
    "CProfileToneHandler",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, NewLTestL, Teardown )
EUNIT_TEST(
    "NewLCTestL",
    "CProfileToneHandler",
    "NewLC",
    "FUNCTIONALITY",
    EmptySetupL, NewLCTestL, Teardown )
EUNIT_TEST(
    "SetProfileToneTestL",
    "CProfileToneHandler",
    "SetProfileToneL",
    "FUNCTIONALITY",
    SetupL, SetProfileToneTestL, Teardown )
EUNIT_TEST(
    "SetActiveProfileRingingToneTestL",
    "CProfileToneHandler",
    "SetActiveProfileRingingToneL",
    "FUNCTIONALITY",
    SetupL, SetActiveProfileRingingToneTestL, Teardown )
EUNIT_TEST(
    "SetToneForAllProfilesTestL",
    "CProfileToneHandler",
    "SetToneForAllProfilesL",
    "FUNCTIONALITY",
    SetupL, SetToneForAllProfilesTestL, Teardown )    

EUNIT_END_TEST_TABLE

//  End of File
