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
* Description:  Setting item class for Feedback settings.
*
*/



#ifndef CPROFILEFEEDBACKSETTINGITEM_H
#define CPROFILEFEEDBACKSETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CAknSettingPage;

// CLASS DECLARATION
/**
*  Setting item class for Feedback settings.
*  CProfileFeedbackSettingItem extends CAknEnumeratedTextPopupSettingItem
*  in order to launch CProfileFeedbackSettingPage
*  instead of CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfileFeedbackSettingItem ) : public CAknEnumeratedTextPopupSettingItem
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aIdentifier Setting item identifier.
        * @param aValue Feedback setting value.
        */
		CProfileFeedbackSettingItem(
			TInt aIdentifier,
			TInt& aValue );

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

        // Setting item identifier
        TInt iIdentifier;
	};

#endif // CPROFILEFEEDBACKSETTINGITEM_H

// End of File
