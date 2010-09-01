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
* Description:  Setting item class for keypad volume setting.
*
*/



#ifndef CPROFILEKEYPADVOLUMESETTINGITEM_H
#define CPROFILEKEYPADVOLUMESETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CAknSettingPage;
class CAknKeySoundSystem;

// CLASS DECLARATION
/**
*  Setting item class for keypad volume setting.
*  CProfileKeypadVolumeSettingItem extends CAknEnumeratedTextPopupSettingItem
*  in order to launch
*  CProfileKeypadVolumeSettingPage instead of CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileKeypadVolumeSettingItem )
 : public CAknEnumeratedTextPopupSettingItem
	{
	public:		// Constructor

        /**
        * C++ constructor.
        */
		CProfileKeypadVolumeSettingItem(
			TInt aIdentifier,
			TInt& aValue,
			CAknKeySoundSystem* aSoundSystem );

	private:	// Functions from base classes

        /**
        * From CAknEnumeratedTextPopupSettingItem
        */
		void CompleteConstructionL();

        /**
        * From CAknEnumeratedTextPopupSettingItem
        */
		CAknSettingPage* CreateSettingPageL();

	private:	// Data

		// Ref: Pointer to AVKON key sound system
		CAknKeySoundSystem* iSoundSystem;

	};

#endif // CPROFILEKEYPADVOLUMESETTINGITEM_H

// End of File
