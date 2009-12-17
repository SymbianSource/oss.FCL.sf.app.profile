/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A wrapper interface around MMF (Multimedia Framework) to get
*                information about MIME-types supported by MMF.
*
*/



#ifndef PROFILEMMFINFOUTILITY_H
#define PROFILEMMFINFOUTILITY_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  A helper interface used by CProfileToneHandler to check whether a MIME
*  type is supported by MMF.
*
*  @lib ProfileSettingsView.lib
*  @since Series 60 3.0
*/
class ProfileMmfInfoUtility
    {
    public: // New functions

        /**
         * Checks if the given MIME-type is supported by MMF. In other words,
         * checks if there is an MMF plugin to handle the given MIME type.
         * @param aMimeType the MIME type for which an MMF plugin is searched.
         * @return ETrue if the given MIME is supported by MMF,
         *         EFalse otherwise.
         */
        static TBool IsMimeTypeSupportedL( const TDesC8& aMimeType );

    private:  // Constructor and destructor

        /**
         * Default constructor. This is private since this class is not intended
         * to be instantiated.
         */
        ProfileMmfInfoUtility() {};

        // Destructor.
        ~ProfileMmfInfoUtility() {};
    };

#endif      // PROFILEMMFINFOUTILITY_H

// End of File
