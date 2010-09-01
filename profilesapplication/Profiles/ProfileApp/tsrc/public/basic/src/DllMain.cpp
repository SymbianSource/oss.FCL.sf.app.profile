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
* Description:  Profile Settings View module test suite and test class
*                creation and DLL entry point function.
*
*/



// INCLUDE FILES
#include "T_CProfileToneHandler.h"
#include <CEUnitTestSuite.h>

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CreateTestSuiteL EUnit test suite factory function.
//                  Creates the test suite and the test classes.
//
// Returns: EUnit test suite interface
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* mainSuite = CEUnitTestSuite::NewLC(
        _L("ProfileSettingsView Module Tests") );

    // NewLC leaves the pointer to cleanupstack until AddL finishes
    mainSuite->AddL( T_CProfileToneHandler::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( mainSuite );
    return mainSuite;
    }

//  End of File
