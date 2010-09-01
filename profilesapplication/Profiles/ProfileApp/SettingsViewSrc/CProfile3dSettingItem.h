/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Setting item class for 3D settings.
*
*/



#ifndef CPROFILE3DSETTINGITEM_H
#define CPROFILE3DSETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CAknSettingPage;

// CLASS DECLARATION
/**
*  Setting item class for 3D settings.
*  CProfile3dSettingItem extends CAknEnumeratedTextPopupSettingItem
*  in order to launch CProfile3dSettingPage
*  instead of CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfile3dSettingItem ) : public CAknEnumeratedTextPopupSettingItem
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aIdentifier Setting item identifier.
        * @param aRingingVolume Ringing volume.
        * @param aRingingTone Ringing tone.
        * @param aRingingType Ringing type.
        * @param aVibratingAlert Vibrating alert.
        * @param aVolume Volume.
        * @param a3DEffectOrEcho 3D effect/echo
        */
		CProfile3dSettingItem(
			TInt aIdentifier,
			TInt& aValue,
			const TDesC& aRingingTone,
			const TInt& aRingingType,
			const TBool& aVibratingAlert,
			const TInt& aVolume,
			const TInt& a3DEffectOrEcho );

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

		// Ringing tone.
		const TDesC& iRingingTone;

		// Ringing type.
		const TInt& iRingingType;

		// Vibrating alert.
		const TBool& iVibratingAlert;

		// Volume.
		const TInt& iVolume;

		// 3D effect or echo
        const TInt& i3DEffectOrEcho;

        // Setting item identifier
        TInt iIdentifier;
	};

#endif // CPROFILE3DSETTINGITEM_H

// End of File
