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
* Description:  Panic codes of Profiles application.
*
*/



#ifndef PROFILEPANICS_H
#define PROFILEPANICS_H

/**
* Enumerations for panics of Profiles application.
*/
enum TProfilePanics
	{
	EProfilePanicInvalidSettingItemId,
	EProfilePanicProfileIndexOutOfRange,
	EProfilePanicNoProfiles
	};

_LIT( KProfilePanicText, "ProfileApp" );

#endif // PROFILEPANICS_H

// End of File
