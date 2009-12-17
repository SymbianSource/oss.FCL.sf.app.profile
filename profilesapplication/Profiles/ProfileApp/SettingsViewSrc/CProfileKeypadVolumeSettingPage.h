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
* Description:  Setting page class for keypad volume setting.
*
*/



#ifndef CPROFILEKEYPADVOLUMESETTINGPAGE_H
#define CPROFILEKEYPADVOLUMESETTINGPAGE_H

// INCLUDES
#include <aknpopupsettingpage.h>
#include <mdaaudiotoneplayer.h>

// FORWARD DECLARATIONS
class CAknKeySoundSystem;

// CLASS DECLARATION
/**
*  Setting page class for keypad volume setting.
*  CProfileKeypadVolumeSettingPage extends CAknPopupSettingPage in order to
*  play a preview sound of the selected keypad volume when changing focus
*  in the setting list.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
class CProfileKeypadVolumeSettingPage : public CAknPopupSettingPage
									  , public MMdaAudioToneObserver
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource id.
        * @param aQueryValue
        * @param aSoundSystem Pointer to AVKON key sound system.
        */
		CProfileKeypadVolumeSettingPage(
			TInt aResourceID,
			MAknQueryValue& aQueryValue,
			CAknKeySoundSystem* aSoundSystem );

		/**
		* Destructor.
		*/
		~CProfileKeypadVolumeSettingPage();

		/**
		* Symbian 2nd phase constructor.
		*/
		void ConstructL();

	private:	// Function from base classes

        /**
        * From CCoeControl
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * From CAknSettingPage
        */
		TBool OkToExitL( TBool aAccept );

		/**
        * From MMdaAudioToneObserver
        */
		void MatoPrepareComplete( TInt aError );

        /**
        * From MMdaAudioToneObserver
        */
		void MatoPlayComplete( TInt aError );

	private:	// Data

		// Ref: Pointer to AVKON key sound system.
		CAknKeySoundSystem* iSoundSystem;

		// Own: Media client to play audio.
		CMdaAudioToneUtility* iAudioUtility;

		// Original volume value.
		TInt iOriginalVolume;

	};

#endif // CPROFILEKEYPADVOLUMESETTINGPAGE_H

// End of File
