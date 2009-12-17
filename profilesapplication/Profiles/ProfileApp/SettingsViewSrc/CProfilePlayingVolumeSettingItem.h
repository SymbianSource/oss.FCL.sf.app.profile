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
* Description:  Setting item class for ringing volume setting.
*
*/



#ifndef CPROFILEPLAYINGVOLUMESETTINGITEM_H
#define CPROFILEPLAYINGVOLUMESETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Setting item class for ringing volume setting.
*  CProfilePlayingVolumeSettingItem extends CAknVolumeSettingItem
*  in order to launch CProfilePlayingVolumeSettingPage
*  instead of CAknVolumeSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfilePlayingVolumeSettingItem ) : public CAknVolumeSettingItem
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aIdentifier Setting item identifier.
        * @param aRingingVolume Ringing volume.
        * @param aRingingTone Ringing tone.
        * @param aRingingType Ringint type.
        * @param aVibratingAlert Vibrating alert.
        * @param aDisplayQuery Information of whether 
		* CProfilePlayingVolumeSettingPage
		* should display confirmation note
		* when setting maximum volume
        */
		CProfilePlayingVolumeSettingItem(
			TInt aIdentifier,
			TInt& aRingingVolume,
			const TDesC& aRingingTone,
			const TInt& aRingingType,
			const TBool& aVibratingAlert,
			const TBool aDisplayQuery );

	private:	// Functions from base classes

        /**
        * From CAknVolumeSettingItem
        */
		void EditItemL( TBool aCalledFromMenu );

	private:	// Data

		// Ringing tone.
		const TDesC& iRingingTone;

		// Ringing type.
		const TInt& iRingingType;

		// Vibrating alert.
		const TBool& iVibratingAlert;
		
		// Display confirmation note.
		const TBool iDisplayQuery;

	};

#endif // CPROFILEPLAYINGVOLUMESETTINGITEM_H

// End of File
