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



#ifndef CPROFILEFILELISTSETTINGITEM_H
#define CPROFILEFILELISTSETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CCoeEnv;


// CLASS DECLARATION
/**
*  Setting item class for tone selection.
*  CProfileFileListSettingItem launches File List Dialog common component
*  from which user can select a file for ringing tone.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileFileListSettingItem ) : public CAknSettingItem
	{

	public:		// Constructors and destructor

		/**
		* C++ constructor.
		* @param aIdentifier Resource identifier.
		*        Tells which setting item is currently edited.
		* @param aToneFileName File name of the selected tone is stored here.
		* @param aRingingType Describes how the tone is played.
		* @param aRingingVolume The volume how loud the tone is played.
		* @param aVibratingAlert Setting of the vibrating alert.
		* @param aCoeEnv Pointer to Control Environment.
		*/
		CProfileFileListSettingItem(
			TInt aIdentifier,
			TDes& aToneFileName,
			const TInt& aRingingType,
			const TInt& aRingingVolume,
			const TBool& aVibratingAlert,
			TInt& a3DEffect,
			TInt& a3DEcho,
			CCoeEnv* aCoeEnv );

        /**
        * Destructor.
        */
		~CProfileFileListSettingItem();

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


	private:	// Data

		// Internal buffer for filename of selected tone
		TFileName* iInternalToneFileName;

		// External filename for selected tone
		TDes& iExternalToneFileName;

		// Ringing type for tone playback
		const TInt& iRingingType;

		// Ringing volume for tone playback
		const TInt& iRingingVolume;

		// Vibrating alert setting for tone playback
		const TBool& iVibratingAlert;

		// Own: Localised text qtn_tc_none
		HBufC* iNoSoundText;

		// Own: "No_Sound.wav" file name
		HBufC* iNoSoundFileName;

		// Own: Localised text for title for tone selection
		HBufC* iTitle; /// Title for tone selection

		// Text that is returned by SettingTextL
		TPtrC iSettingText;

		TInt& i3DEffect;
		TInt& i3DEcho;
		// Ref: Pointer to Control Environment
		CCoeEnv* iCoeEnv;

	};

#endif // CPROFILEFILELISTSETTINGITEM_H

// End of File
