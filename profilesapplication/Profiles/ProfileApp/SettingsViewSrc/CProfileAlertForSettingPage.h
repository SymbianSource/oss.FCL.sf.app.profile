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
* Description:  Setting page class for "Alert for" setting.
*
*/



#ifndef CPROFILEALERTFORSETTINGPAGE_H
#define CPROFILEALERTFORSETTINGPAGE_H

// INCLUDES
#include <akncheckboxsettingpage.h>

// CLASS DECLARATION
/**
*  Setting page class for "Alert for" setting.
*  CProfileAlertForSettingPage extends CAknCheckBoxSettingPage in order to
*  hide left softkey when no contact groups are selected. However, it is
*  the responsibility of the setting page observer to call UpdateCbaL, when
*  setting changes.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileAlertForSettingPage )
 : public CAknCheckBoxSettingPage
	{

	public:		// Constructor

        /**
        * C++ constructor.
        * @param aResourceID Setting page resource id.
        * @param aItemArray The array which contains all setting items.
        */
        CProfileAlertForSettingPage(
			TInt aResourceID, CSelectionItemList* aItemArray );

	public:		// New functions

		/**
		* Hides left softkey (Ok) if no items are selected.
		*/
		void UpdateCba();

	};

#endif // CPROFILEALERTFORSETTINGPAGE_H

// End of File
