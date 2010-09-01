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
* Description:  Data structure class for profile tones view settings.
*
*/



#ifndef CPROFILESGSTONESVIEWSETTINGS_H
#define CPROFILESGSTONESVIEWSETTINGS_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CProfilesGSTonesViewFeatures;
class CRepository;

// CLASS DECLARATION
/**
*  Data structure class for Profile GS Tones View settings. Contains only  the
*  settings not found in Profiles Personalization view. However, Calendar
*  and Clock alert tones are not handled by this class.
*
*  @lib ProfilesGSTonesPlugin.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProfilesGSTonesViewSettings ) : public CBase
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfilesGSTonesViewSettings* NewL(
                CProfilesGSTonesViewFeatures& aFeatures );

        /**
        * Destructor.
        */
        ~CProfilesGSTonesViewSettings();

    public:     // New functions

        /**
        * Stores the value of the given setting to persistent memory if the
        * value has been changed (from that found from persistent storage).
        * @param aSettingId the ID of the setting (as defined in resources) to
        *        be checked.
        */
        void StoreIfChangedL( TInt aSettingId ) const;

    private:
        /**
        * C++ constructor.
        */
        CProfilesGSTonesViewSettings( CProfilesGSTonesViewFeatures& aFeatures )
            : iFeatures( aFeatures ) {};

        /**
        * Symbian constructor.
        */
        void ConstructL();

    public:     // Data

        // HAC setting
        TBool iHac;

        // HW Resource Manager central repository
        // Note, this is NULL if HAC is not supported:
        CRepository* iHwRmSession;

        // Reference to the variable features wrapper
        CProfilesGSTonesViewFeatures& iFeatures;
    };

#endif // CPROFILESGSTONESVIEWSETTINGS_H

// End of File
