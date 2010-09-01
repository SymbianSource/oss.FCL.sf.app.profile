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
* Description:  Data structure class for profile settings.
*
*/



#ifndef CPROFILESTRUCT_H
#define CPROFILESTRUCT_H

// INCLUDES
#include "ProfileEng.hrh"
#include "ProfileInternal.hrh"
#include <cntdef.h>

// CONSTANTS
const TInt KProfileMinVolumeLevel( 1 );
const TInt KProfileMaxVolumeLevel( 10 );
const TInt KProfileMaxProfileNameLength( PROFILES_MAX_NAME_LENGTH );

// FORWARD DECLARATIONS
class MProfileExtended;

// CLASS DECLARATION
/**
*  Data structure class for profile settings.
*  Holds settings for one profile.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileStruct ) : public CBase
    {
    public:     // Constructors and destructor

		/**
		 * Creates and returns a new instance of this class.
		 * @return Pointer to the CProfileStruct object
		 */
		static CProfileStruct* NewL( );

        /**
        * Destructor.
        */
        virtual ~CProfileStruct();
	
	private:	// Constructors
    	/**
        * C++ default constructor.
        */
        CProfileStruct();
        
        /**
        * Second phase constructor
        */
        void ConstructL();
        
    public:     // New functions

        /**
        * Loads settings from Profile Engine via MProfileExtended interface to member data.
        * @param aProfile Pointer to Profile Engine interface.
        */
        void LoadL( MProfileExtended* aProfile );

        /**
        * Stores member data to Profile Engine via MProfileExtended interface.
        * @param aProfile Pointer to Profile Engine interface.
        */
        void StoreL( MProfileExtended* aProfile );

		/**
		* Returns ETrue if member data is the same as in aProfile.
		* @param aProfile Profile to compare member data with.
		* @return Returns ETrue if member data is the same as in aProfile.
		*/
		TBool CompareL( MProfileExtended* aProfile ) const;

        /**
        * Compares if any ringing/alarm tone setting has changed and
        * removes it from the automated content list using DRM Helper.
        * @param aProfile Profile to compare member data with.
        */
        void RemoveAutomatedContentL( MProfileExtended* aProfile ) const;

    public:     // Data

        // ID of the profile
        TInt iProfileId;

        // Filename of ringing tone or line 1 ringing tone
        TFileName* iRingingTone;

        // Filename of line 2 ringing tone
        TFileName* iRingingToneLine2;

        // Filename of video call tone
        TFileName* iVideoCallTone;

        // Ringing type
        TInt iRingingType;

        // Ringing volume (between KProfileMinVolumeLevel and KProfileMaxVolumeLevel)
        TInt iRingingVolume;

		// Media volume (between KProfileMinVolumeLevel and KProfileMaxVolumeLevel)
		TInt iMediaVolume;

        // Message alert tone
        TFileName* iMessageAlertTone;

        // E-mail alert tone
        TFileName* iEmailAlertTone;

        // Vibrating alert setting
        TBool iVibratingAlert;

        // Email Vibrating alert setting
         TBool iEmailVibratingAlert;
        // Keypad volume level
        TInt iKeypadVolume;

        // Warning and game tones setting
        TBool iWarningAndGameTones;

        // Alert for setting
        RArray<TContactItemId> iContactList;

        // Profile name
        TBuf<KProfileMaxProfileNameLength> iProfileName;

        // Profile name for title pane
        TBuf<KProfileMaxProfileNameLength> iTitleName;

		// TTS Ringingtone setting
		TBool iTextToSpeech;

		// Filename of PTT ringing tone
        TFileName* iPTTRingingTone;

        // PTT Status
        TInt iPTTStatus;

        // Tactile feedback
        TInt iTactileFeedback;

        // Audio feedback
        TInt iAudioFeedback;
        
        // 3D effect
        TInt i3DEffect;

        // 3D echo
        TInt i3DEcho;
        
    };

#endif // CPROFILESTRUCT_H

// End of File
