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
* Description:  Application UI class.
*
*/


// INCLUDE FILES
#include "CProfileAppUi.h"

#include <hlplch.h>
#include <CProfileEngineHandler.h>

#include "CProfileMainView.h"
#include "CProfileSettingsView.h"
#include "ProfileApp.hrh"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileAppUi::CProfileAppUi
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileAppUi::CProfileAppUi(
    CProfileEngineHandler& aEngineHandler,
    CProfileIndexHandler& aIndexHandler )
    :   iEngineHandler( aEngineHandler ),
        iIndexHandler( aIndexHandler )
    {
    }

// Destructor
CProfileAppUi::~CProfileAppUi()
    {
    }

// -----------------------------------------------------------------------------
// CProfileAppUi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileAppUi::ConstructL()
    {
    BaseConstructL( EAknEnableSkin | EAknEnableMSK |EAknSingleClickCompatible);

	CProfileMainView* mainView = 
        CProfileMainView::NewLC( iEngineHandler, iIndexHandler );
    AddViewL( mainView ); // transfer ownership to CAknViewAppUi
    SetDefaultViewL( *mainView );
    CleanupStack::Pop( mainView);
    iMainView = mainView;

	CProfileSettingsView* settingsView =
	 CProfileSettingsView::NewLC(
        iEngineHandler, iIndexHandler);
    AddViewL( settingsView ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( settingsView ); // iSettingsView
    iSettingsView = settingsView;
    }

// -----------------------------------------------------------------------------
// CProfileAppUi::HandleCommandL
// -----------------------------------------------------------------------------
//
void CProfileAppUi::HandleCommandL( TInt aCommand )
    {
    switch( aCommand )
        {
        case EAknSoftkeyBack: // Back from Settings view
            {
            if ( iView == iMainView )
                {
                Exit();  // Back from Settings view
                break;
                }
            ActivateLocalViewL( iMainView->Id() );
            break;
            }
        case EProfileCmdPersonalise: // Personalise from Main view
            {
            ActivateLocalViewL( iSettingsView->Id() );
            break;
            }
        // FALLTHROUGH for PC Lint
        // Exit from options menu and exit from right softkey do the same thing.
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            Exit();
            break;
            }
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL(
                iEikonEnv->WsSession(), AppHelpContextL() );
            break;
            }
        default:
            {
            break;
            }
        }
    }

// End of File
