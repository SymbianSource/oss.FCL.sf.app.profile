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
* Description:  Setting item class for tone selection.
*
*/



#ifndef CPROFILESGSTONESPLUGINFILELISTSETTINGITEM_H
#define CPROFILESGSTONESPLUGINFILELISTSETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>
#include <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS
class CCoeEnv;
class CFLDFileListContainer;
class CRepository;
class MProfilesGSTonesPluginItemDataInterface;

// CLASS DECLARATION
/**
*  Setting item class for tone selection.
*  CProfilesGSTonesPluginFileListSettingItem launches File List Dialog common component
*  from which user can select a file for ringing tone.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfilesGSTonesPluginFileListSettingItem ) : public CAknSettingItem,
    public MCenRepNotifyHandlerCallback
	{

	public:		// Constructors and destructor

		/**
		* C++ constructor.
		* @param aIdentifier Resource identifier.
		*        Tells which setting item is currently edited.
		* @param aRingingType Describes how the tone is played.
		* @param aRingingVolume The volume how loud the tone is played.
		* @param aVibratingAlert Setting of the vibrating alert.
		* @param aCoeEnv Pointer to Control Environment.
		* @param aDataInterface reference to the interface to the data
        *        storage.
		*/
		CProfilesGSTonesPluginFileListSettingItem(
			TInt aIdentifier,
            const TDesC& aDefaultToneFileName,
			CCoeEnv* aCoeEnv,
            MProfilesGSTonesPluginItemDataInterface& aDataInterface );

        /**
        * Destructor.
        */
		~CProfilesGSTonesPluginFileListSettingItem();

	private:	// Functions from base classes

        /**
        * From CAknSettingItem
        */
		void CompleteConstructionL();

        /**
        * From CAknSettingItem
        */
		void StoreL();

        /**
        * From CAknSettingItem
        */
		void LoadL();

        /**
        * From CAknSettingItem
        */
		void EditItemL( TBool aCalledFromMenu );

        /**
        * From CAknSettingItem
        */
		const TDesC& SettingTextL();

        /**
        * From MCenRepNotifyHandlerCallback
        */
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        
	private: // New functions
	     /**
         * Read the size limitation key from Profiles Central Repository.
		 *
		 * @param aRepository reference to an open Profiles Cenrep session.
         */
        void ReadMaxToneFileSizeL( CRepository& aRepository );

	private:	// Data

		// Internal buffer for filename of selected tone
		TFileName iInternalToneFileName;

		// Ringing type for tone playback
		TInt iRingingType;

		// Ringing volume for tone playback
		TInt iRingingVolume;

		// Vibrating alert setting for tone playback
		TBool iVibratingAlert;

		// Own: Localised text qtn_tc_none
		HBufC* iNoSoundText;

		// Own: "No_Sound.wav" file name
		HBufC* iNoSoundFileName;

		// Own: Localised text qtn_no_personal_tone
		HBufC* iDefaultToneText;

        // Default tone file name
        TFileName iDefaultToneFileName;

		// Own: Localised text for title for tone selection
		HBufC* iTitle; /// Title for tone selection

		// Text that is returned by SettingTextL
		TPtrC iSettingText;

		// Ref: Pointer to Control Environment
		CCoeEnv* iCoeEnv;

        // Own: File list container
        CFLDFileListContainer* iFileListContainer;

        // Own: Notification handler for active profile vibra setting
        CCenRepNotifyHandler* iVibraNotifyHandler;

        // Own: Notification handler for active profile ringing type setting
        CCenRepNotifyHandler* iTypeNotifyHandler;

        // Own: Notification handler for active profile ringing volume setting
        CCenRepNotifyHandler* iVolumeNotifyHandler;

        // Own: Profiles Central Repository session
        CRepository* iSession;

        // Own: Vibra control Central Repository session
        CRepository* iVibraSession;

        // The interface to the external data storage
        MProfilesGSTonesPluginItemDataInterface& iDataInterface; 

        // Holds the size limitation value for ringing tone file size
        TInt iSizeLimit;
	};

#endif // CPROFILESGSTONESPLUGINFILELISTSETTINGITEM_H

// End of File
