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
* Description:  Test class for ProfileToneHandler.
*
*/


#ifndef T_CPROFILETONEHANDLER_H
#define T_CPROFILETONEHANDLER_H

//  INCLUDES
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class CProfileToneHandler;

// CLASS DECLARATION

/**
*   Test class for CProfileToneHandler.
*
*  @lib SettingsViewMdlTest.lib
*  @since 2.5
*/
class   T_CProfileToneHandler : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructor

        static T_CProfileToneHandler* NewLC();
        ~T_CProfileToneHandler();

    private:

        void SetupL();

        void EmptySetupL();

        void Teardown();

        void NewLTestL();

        void NewLCTestL();

	void SetActiveProfileRingingToneTestL();

	void SetProfileToneTestL();
	
	void SetToneForAllProfilesTestL();

    private:

        T_CProfileToneHandler();
        void ConstructL();

    private:    // Data

        // Own: The tested class, Profile tone Handler.
        CProfileToneHandler* iToneHandler;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  T_CPROFILETONEHANDLER_H

// End of File
