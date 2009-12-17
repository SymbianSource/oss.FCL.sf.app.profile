/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Wraps  the information about variable features of GS Tones
*                View.
*
*/



#ifndef CPROFILESGSTONESVIEWFEATURES_H
#define CPROFILESGSTONESVIEWFEATURES_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* Contains the information about variated features of GS Tones View.
*
*  @lib ProfilesGSTonesPlugin.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProfilesGSTonesViewFeatures ) : public CBase
    {
    public:     // Constructor and destructor

        static CProfilesGSTonesViewFeatures* NewL();

        /**
        * Destructor.
        */
        ~CProfilesGSTonesViewFeatures();

    public:     // New functions

        /**
        * Checks if the given locally variated feature is enabled.
        * @param aFeatureId the feature id Tones view features as specified in
        *        ProfileEngineDomainCRKeys.h
        * @return ETrue is the given feature is supported, false otherwise.
        */
        TBool IsLocalFeatureSupported( TUint32 aFeatureId ) const;

    private:     // constructors
        /**
        * C++ default constructor.
        */
        CProfilesGSTonesViewFeatures() {};

        /**
         * Symbian constructor.
         */
        void ConstructL();

    private:     // Data

        // Local features of Tones View
        TInt iLocalFeatures;

    };

#endif // CPROFILESGSTONESVIEWFEATURES_H

// End of File

