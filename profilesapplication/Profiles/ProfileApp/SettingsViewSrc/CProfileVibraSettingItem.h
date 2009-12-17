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
* Description:  Setting item class for Vibra settings.
*
*/



#ifndef CPROFILEVIBRASETTINGITEM_H
#define CPROFILEVIBRASETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CAknSettingPage;

// CLASS DECLARATION
/**
*  Setting item class for Vibra settings.
*  CProfileVibraSettingItem extends CAknBinaryPopupSettingItem
*  in order to launch CProfileVibraSettingPage
*  instead of CAknPopupSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CProfileVibraSettingItem ) : public CAknBinaryPopupSettingItem
	{
	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aIdentifier Setting item identifier.
        * @param aValue Vibra setting value.
        */
		CProfileVibraSettingItem(
			TInt aIdentifier,
			TBool& aValue );

	private:	// Functions from base classes

        /**
        * From CAknBinaryPopupSettingItem
        */
		CAknSettingPage* CreateSettingPageL();

	private:	// Data

	};

#endif // CPROFILEVIBRASETTINGITEM_H

// End of File
