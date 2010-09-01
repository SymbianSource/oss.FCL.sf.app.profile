/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Debug macros for Profiles.
*
*/


#ifndef PROFILESDEBUG_H
#define PROFILESDEBUG_H

#include <e32debug.h>

#ifdef _DEBUG
#define PRODEBUG(a) RDebug::Print( _L( a ) )
#define PRODEBUG1(a,b) RDebug::Print( _L( a ),b )
#define PRODEBUG2(a,b,c) RDebug::Print( _L( a ),b,c )
#define PRODEBUG3(a,b,c,d) RDebug::Print( _L( a ),b,c,d )
#else
#define PRODEBUG(a) 
#define PRODEBUG1(a,b) 
#define PRODEBUG2(a,b,c) 
#define PRODEBUG3(a,b,c,d) 
#endif // _DEBUG

#endif // PROFILESDEBUG_H

// End of File

