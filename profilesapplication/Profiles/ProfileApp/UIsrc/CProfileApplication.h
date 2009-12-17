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
* Description:  Application class.
*
*/



#ifndef CPROFILEAPPLICATION_H
#define CPROFILEAPPLICATION_H

// INCLUDES
#include <aknapp.h>
#include "CProfileDocument.h"

// CONSTANTS
const TUid KUidProfileApp = { 0x100058F8 }; // UID of the application

// CLASS DECLARATION
/**
*  Application class.
*
*  @lib ProfileApp.app
*  @since 1.2
*/
class CProfileApplication : public CAknApplication
	{
	private:	// Functions from base classes

        /**
        * From CEikApplication
        */
		CApaDocument* CreateDocumentL() { return( CProfileDocument::NewL( *this ) ); }

        /**
        * From CApaApplication
        */
		TUid AppDllUid() const { return KUidProfileApp; }

	};

#endif // CPROFILEAPPLICATION_H

// End of File
