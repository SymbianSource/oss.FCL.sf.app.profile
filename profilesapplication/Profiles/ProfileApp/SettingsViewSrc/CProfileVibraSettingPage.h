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
* Description:  Setting page class for Vibra settings.
*
*/



#ifndef CPROFILEVIBRASETTINGPAGE_H
#define CPROFILEVIBRASETTINGPAGE_H

// INCLUDES
#include <aknpopupsettingpage.h>

// FORWARD DECLARATIONS
class CFLDController;

// CLASS DECLARATION
/**
*  Setting page class for Vibra settings.
*  CProfileVibraSettingPage extends CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfileVibraSettingPage ) : public CAknPopupSettingPage
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource ID.
        * @param aQueryValue
        */
		CProfileVibraSettingPage(
			TInt aResourceID,
			MAknQueryValue& aQueryValue );

        /**
        * Symbian 2nd phase constructor.
        */
		void ConstructL();

		/**
		* Destructor.
		*/
		~CProfileVibraSettingPage();

	private:	// Functions from base classes

        /**
        * From CAknSettingPage
        */
		TBool OkToExitL( TBool aAccept );

	private:	// New functions
	
	private:	// Data

	};

#endif // CPROFILEVIBRASETTINGPAGE_H

// End of File
