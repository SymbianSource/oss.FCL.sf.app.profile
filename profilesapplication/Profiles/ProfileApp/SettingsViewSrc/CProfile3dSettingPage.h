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
* Description:  Setting page class for 3D settings.
*
*/



#ifndef CPROFILE3DSETTINGPAGE_H
#define CPROFILE3DSETTINGPAGE_H

// INCLUDES
#include <aknpopupsettingpage.h>

// FORWARD DECLARATIONS
class CFLDController;

// CLASS DECLARATION
/**
*  Setting page class for 3D settings.
*  CProfile3dSettingPage extends CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfile3dSettingPage ) : public CAknPopupSettingPage
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource ID.
        * @param aQueryValue
        * @param aRingingTone Ringing tone.
        * @param aRingingType Ringing type.
        * @param aVibratingAlert Vibrating alert.
        * @param aVolume Volume.
        * @param a3DEffectOrEcho 3D effect or echo.
        * @param aIdentifier Setting item identifier.
        */
		CProfile3dSettingPage(
			TInt aResourceID,
			MAknQueryValue& aQueryValue,
			const TDesC& aRingingTone,
			const TInt& aRingingType,
			const TBool& aVibratingAlert,
			const TInt& aVolume,
			const TInt& a3DEffectOrEcho,
			TInt aIdentifier );

        /**
        * Symbian 2nd phase constructor.
        */
		void ConstructL();

		/**
		* Destructor.
		*/
		~CProfile3dSettingPage();

	private:	// Functions from base classes

        /**
        * From CCoeControl
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);

	private:	// New functions
	
		/**
		* Checks if ringing tone is video and if so, sets default tone
		*/
		void CheckRingingToneTypeL();

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

        // Temporary setting value
        TInt iTempValue;

		// Own: File List Dialog
		CFLDController* iController;
		
		// Preview tone: either ringing tone or default tone
		HBufC* iPreviewTone;

        // Fileserver session
        RFs* iFs;
        
	};

#endif // CPROFILE3DSETTINGPAGE_H

// End of File
