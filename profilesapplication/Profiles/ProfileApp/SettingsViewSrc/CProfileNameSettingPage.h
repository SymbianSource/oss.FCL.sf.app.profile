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
* Description:  Setting page class for profile name setting.
*
*/



#ifndef CPROFILENAMESETTINGPAGE_H
#define CPROFILENAMESETTINGPAGE_H

// INCLUDES
#include "CProfileStruct.h" // KProfileMaxProfileNameLength
#include <akntextsettingpage.h>

// FORWARD DECLARATIONS
class CProfileEngineHandler;

// CLASS DECLARATION
/**
*  Setting page class for profile name setting.
*  CProfileNameSettingPage extends CAknTextSettingPage in order to check
*  validity of user typed profile name.
*  Error note is shown if entered profile name already exists.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileNameSettingPage ) : public CAknTextSettingPage
	{
	public:		// Constructors and destructor

		/**
		* C++ constructor.
		* @param aResourceID Setting page resource ID.
		* @param aText The new profile name will be stored here.
		* @param aTextSettingPageFlags Flags for the setting page.
        * @param aEngineHandler Profile Engine Handler.
		*/
		CProfileNameSettingPage(
			TInt aResourceID,
			TDes& aText,
			TInt aTextSettingPageFlags,
            CProfileEngineHandler& aEngineHandler );

	private:	// Functions from base classes

        /**
        * From CAknSettingPage
        */
		TBool OkToExitL( TBool aAccept );

	private:	// Data

		/// Profile Engine Handler;
		CProfileEngineHandler& iEngineHandler;

        /// The original profile name
		TBuf<KProfileMaxProfileNameLength> iOriginalText;

	};

#endif // CPROFILENAMESETTINGPAGE_H

// End of File
