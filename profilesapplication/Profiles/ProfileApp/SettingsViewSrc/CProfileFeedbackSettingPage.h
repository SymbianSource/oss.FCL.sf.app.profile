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
* Description:  Setting page class for Feedback settings.
*
*/



#ifndef CPROFILEFEEDBACKSETTINGPAGE_H
#define CPROFILEFEEDBACKSETTINGPAGE_H

// INCLUDES
#include <aknpopupsettingpage.h>

// FORWARD DECLARATIONS
class CFLDController;

// CLASS DECLARATION
/**
*  Setting page class for Feedback settings.
*  CProfileFeedbackSettingPage extends CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfileFeedbackSettingPage ) : public CAknPopupSettingPage
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource ID.
        * @param aQueryValue
        * @param aIdentifier Setting item identifier.
        */
		CProfileFeedbackSettingPage(
			TInt aResourceID,
			MAknQueryValue& aQueryValue,
			TInt aIdentifier );

        /**
        * Symbian 2nd phase constructor.
        */
		void ConstructL();

		/**
		* Destructor.
		*/
		~CProfileFeedbackSettingPage();

	private:	// Functions from base classes

        /**
        * From CCoeControl
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);

        /**
        * From CAknSettingPage
        */
		TBool OkToExitL( TBool aAccept );

	private:	// New functions
	

	private:	// Data

        // Setting item identifier
        TInt iIdentifier;

        // Temporary setting value
        TInt iTempValue;

	};

#endif // CPROFILEFEEDBACKSETTINGPAGE_H

// End of File
