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
* Description:  Setting page class for ringing volume setting.
*
*/



#ifndef CPROFILEPLAYINGVOLUMESETTINGPAGE_H
#define CPROFILEPLAYINGVOLUMESETTINGPAGE_H

// INCLUDES
#include <aknvolumesettingpage.h>

// FORWARD DECLARATIONS
class CFLDController;

// CLASS DECLARATION
/**
*  Setting page class for ringing volume setting.
*  CProfilePlayingVolumeSettingPage extends CAknVolumeSettingPage in order to
*  play a ringing tone at selected volume and to show a confirmation query
*  if user selects the highest volume level.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfilePlayingVolumeSettingPage ) : public CAknVolumeSettingPage
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource ID.
        * @param aVolume Selected ringing tone will be stored here.
        * @param aRingingTone Ringing tone.
        * @param aRingingType Ringing type.
        * @param aVibratingAlert Vibrating alert.
        * @param aDisplayQuery Info whether to show
        * maximum volume query if volume is set to max
        */
		CProfilePlayingVolumeSettingPage(
			TInt aResourceID,
			TInt& aVolume,
			const TDesC& aRingingTone,
			const TInt& aRingingType,
			const TBool& aVibratingAlert,
			const TBool aDisplayQuery );

        /**
        * Symbian 2nd phase constructor.
        */
		void ConstructL();

		/**
		* Destructor.
		*/
		~CProfilePlayingVolumeSettingPage();

	private:	// Functions from base classes

        /**
        * From CCoeControl
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);

        /**
        * From CAknSettingPage
        */
		TBool OkToExitL(TBool aAccept);

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

		// Own: File List Dialog
		CFLDController* iController;
		
		// Information of whether to display
		// maximum volume query dialog
		const TBool iDisplayQuery;

		// Preview tone: either ringing tone or default tone
		HBufC* iPreviewTone;

        // Fileserver session
        RFs* iFs;
        
        // Temporary volume value
        TInt iTempVolume;

	};

#endif // CPROFILEPLAYINGVOLUMESETTINGPAGE_H

// End of File
