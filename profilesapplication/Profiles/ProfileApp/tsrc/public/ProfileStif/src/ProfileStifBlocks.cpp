/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "ProfileStif.h"
#include <CProfileToneHandler.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileStif::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CProfileStif::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CProfileStif::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CProfileStif::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "NewLTestL", CProfileStif::NewLTestL ),
        ENTRY( "NewLCTestL", CProfileStif::NewLCTestL ),
        ENTRY( "SetActiveProfileRingingToneTestL", CProfileStif::SetActiveProfileRingingToneTestL ),
        ENTRY( "SetProfileToneTestL", CProfileStif::SetProfileToneTestL ),
        ENTRY( "SetToneForAllProfilesTestL", CProfileStif::SetToneForAllProfilesTestL ),
        ENTRY( "SetProfileToneNotCheckLTestL", CProfileStif::SetProfileToneNotCheckLTestL )
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CProfileStif::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//


TInt CProfileStif::NewLTestL( CStifItemParser& aItem )
	{
    CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
    CleanupStack::PushL( toneHandler );
    CleanupStack::PopAndDestroy(); // toneHandler
    return KErrNone;
	}

TInt CProfileStif::NewLCTestL( CStifItemParser& aItem )
	{
    CProfileToneHandler* toneHandler = CProfileToneHandler::NewLC();
    CleanupStack::PopAndDestroy(); // toneHandler
    return KErrNone;
	}

TInt CProfileStif::SetActiveProfileRingingToneTestL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
	CleanupStack::PushL( toneHandler );
	err = toneHandler->SetActiveProfileRingingToneL( KNullDesC );
	CleanupStack::PopAndDestroy(); // toneHandler
	return err;
	}

TInt CProfileStif::SetProfileToneTestL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
	CleanupStack::PushL( toneHandler );
	err = toneHandler->SetProfileToneL( 0, EProfileRingingToneSetting, KNullDesC );
	CleanupStack::PopAndDestroy(); // toneHandler
	return err;
	}

TInt CProfileStif::SetToneForAllProfilesTestL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
	CleanupStack::PushL( toneHandler );
	err = toneHandler->SetToneForAllProfilesL( EProfileRingingToneSetting, KNullDesC );
	CleanupStack::PopAndDestroy(); // toneHandler
	return err;
	}

TInt CProfileStif::SetProfileToneNotCheckLTestL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	CProfileToneHandler* toneHandler = CProfileToneHandler::NewL();
	CleanupStack::PushL( toneHandler );
	err = toneHandler->SetProfileToneNotCheckL(0, EProfileRingingToneSetting, KNullDesC );
	CleanupStack::PopAndDestroy(); // toneHandler
	return err;	
	}

// -----------------------------------------------------------------------------
// CProfileStif::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CProfileStif::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
