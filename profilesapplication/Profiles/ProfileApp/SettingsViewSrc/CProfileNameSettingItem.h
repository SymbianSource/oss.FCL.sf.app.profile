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
* Description:  Setting item class for profile name setting.
*
*/



#ifndef CPROFILENAMESETTINGITEM_H
#define CPROFILENAMESETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS
class CProfileEngineHandler;

// CLASS DECLARATION
/**
*  Setting item class for profile name setting.
*  CProfileNameSettingItem extends CAknTextSettingItem in order to launch
*  CProfileNameSettingPage instead of CCAknTextSettingPage.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileNameSettingItem ) : public CAknTextSettingItem
	{
	public:		// Constructor

		/**
		* C++ constructor.
		* @param aIdentifier Setting item identifier.
		* @param aText A descriptor where new profile name is stored.
		* @param aEngineHandler Profile Engine Handler.
		*/
		CProfileNameSettingItem(
			TInt aIdentifier,
			TDes& aText,
			CProfileEngineHandler& aEngineHandler );

	private:	// Functions from base classes

        /**
        * From CAknSettingItem
        */
		void EditItemL( TBool aCalledFromMenu );

	private:	// Data

		// Profile Engine Handler.
		CProfileEngineHandler& iEngineHandler;

	};

#endif // CPROFILENAMESETTINGITEM_H

// End of File
