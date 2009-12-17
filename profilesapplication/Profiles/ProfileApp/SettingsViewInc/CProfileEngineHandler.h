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



#ifndef CPROFILEENGINEHANDLER_H
#define CPROFILEENGINEHANDLER_H

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class MProfileEngineExtended;
class MProfilesNamesArray;

//  CLASS DECLARATION

/**
*  Profile Engine Handler.
*  Keeps an instance of Profile Engine and maintains a list of profile names.
*
*  @lib ProfileSettingsView.lib
*  @since 2.5
*/
class CProfileEngineHandler : public CBase
    {
    public:     // Constructors and destructor

        /**
        * Symbian 2-phase constructor.
        * @return Returns an instance of CProfileEngineHandler.
        */
        IMPORT_C static CProfileEngineHandler* NewL();

        // Destructor.
        ~CProfileEngineHandler();

    public:     // New methods

        /**
        * Returns pointer to Profile Engine extended interface.
        * @return Returns pointer to Profile Engine extended interface.
        */
        IMPORT_C MProfileEngineExtended* Engine() const;

        /**
        * Reads an array of profile IDs from Profile Engine.
        */
        IMPORT_C void ReadIdArrayL();

        /**
        * Returns an array of profile names and IDs.
        * @return Returns an array of profile names and IDs.
        */
        IMPORT_C MProfilesNamesArray* IdArray() const;

        /**
        * Returns the ID of a profile at index in iProfileIdArray
        * @param aIndex Index of the profile.
        * @return Returns ID of the profile.
        */
        IMPORT_C TInt IdForIndex( TInt aIndex ) const;

    protected:

        // C++ default constructor.
        CProfileEngineHandler();

        // Symbian 2nd phase construct.
        void ConstructL();

    private:    // Data

        // Own: Profile engine extended instance.
        MProfileEngineExtended* iEngine;

        // Own: Array of profile names and IDs.
        MProfilesNamesArray* iIdArray;

    };

#endif // CPROFILEENGINEHANDLER_H

// End of File
