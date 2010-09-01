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
* Description:  Settings handler for the settings outside of Profiles.
*
*/



// CLASS HEADER
#include "CProfilesGSTonesPluginSettingsHandler.h"

// INTERNAL INCLUDES
#include "CProfilesGSTonesPluginFileListSettingItem.h"
#include "ProfilesGSTonesPlugin.hrh"
#include "ProfilesDebug.h"

// INCLUDES FROM PROFILES SETTINGS VIEW
#include <ProfilePanics.h>
#include <ProfileSettingsView.hrh>
#include <MProfileExternalSettingsObserver.h>

// SYSTEM INCLUDES
#include <centralrepository.h>
#include <CalendarInternalCRKeys.h>
#include <ClockDomainCRKeys.h>
#include <ProfileEngineDomainCRKeys.h>
#include <CoeHelp.h>
#include <csxhelp/mode.hlp.hrh>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::CProfilesGSTonesPluginSettingsHandler
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfilesGSTonesPluginSettingsHandler::CProfilesGSTonesPluginSettingsHandler(
        CCoeEnv* aCoeEnv )
    : iCoeEnv( aCoeEnv )
    {
    }

// Destructor
CProfilesGSTonesPluginSettingsHandler::~CProfilesGSTonesPluginSettingsHandler()
    {
    DrmDelete();
	if( iClockNotifyHandler )
		{
		iClockNotifyHandler->StopListening();
		}
	if( iCalendarNotifyHandler )
		{
		iCalendarNotifyHandler->StopListening();
		}

    delete iClockNotifyHandler;
    delete iCalendarNotifyHandler;

    delete iClockSession;
    delete iCalendarSession;
    delete iCalendarInterface;
    delete iClockInterface;

    delete iTonesViewSettings;
    delete iFeatures;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::ConstructL()
    {
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL begin" );
    iFeatures = CProfilesGSTonesViewFeatures::NewL();
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL after features" );
    iTonesViewSettings = CProfilesGSTonesViewSettings::NewL( *iFeatures );
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL after settings" );

    iClockSession = CRepository::NewL( KCRUidClockApp );
    iCalendarSession = CRepository::NewL( KCRUidCalendar );

	iClockNotifyHandler = CCenRepNotifyHandler::NewL( *this,
            *iClockSession,
            CCenRepNotifyHandler::EStringKey,
            KClockAppSoundFile );

    iClockNotifyHandler->StartListeningL();

	iCalendarNotifyHandler = CCenRepNotifyHandler::NewL( *this,
            *iCalendarSession,
            CCenRepNotifyHandler::EStringKey,
            KCalendarSoundFile );

    iCalendarNotifyHandler->StartListeningL();

    User::LeaveIfError( iClockSession->Get( KClockAppSoundFile, iClockTone ) );
    User::LeaveIfError( iCalendarSession->Get( KCalendarSoundFile,
                                               iCalendarTone ) );
    DrmConstructL();

    iCalendarInterface =
        new ( ELeave ) CProfilesGSTonesPluginCalendarDataInterface( *this );
    iClockInterface =
        new ( ELeave ) CProfilesGSTonesPluginClockDataInterface( *this );
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::NewL
// -----------------------------------------------------------------------------
//
CProfilesGSTonesPluginSettingsHandler*
        CProfilesGSTonesPluginSettingsHandler::NewL( CCoeEnv* aCoeEnv )
    {
    CProfilesGSTonesPluginSettingsHandler* self = new ( ELeave )
        CProfilesGSTonesPluginSettingsHandler( aCoeEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::CreateSettingItemL
// -----------------------------------------------------------------------------
//
CAknSettingItem* CProfilesGSTonesPluginSettingsHandler::CreateSettingItemL(
        TInt aIdentifier )
    {
    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::CreateSettingItemL(%d) begin",aIdentifier );
    TFileName defaultToneFile;
    CAknSettingItem* item = NULL;

    switch( aIdentifier )
        {
        case EProfileGSTonesPluginSettingCalendarToneId:
            {
            User::LeaveIfError( iCalendarSession->Get( KCalendarDefaultSoundFile,
                                                       defaultToneFile ) );
            item = new ( ELeave ) CProfilesGSTonesPluginFileListSettingItem(
                   aIdentifier, defaultToneFile, iCoeEnv, *iCalendarInterface );
            break;
            }
        case EProfileGSTonesPluginSettingClockToneId:
            {
            User::LeaveIfError( iClockSession->Get( KClockAppDefaultSoundFile,
                                                    defaultToneFile ) );
            item = new ( ELeave ) CProfilesGSTonesPluginFileListSettingItem(
                   aIdentifier, defaultToneFile, iCoeEnv, *iClockInterface );
                   
            break;
            }
        case EProfileGSTonesPluginSettingHacModeId:
            {
            item = new( ELeave ) CAknBinaryPopupSettingItem(
                aIdentifier, iTonesViewSettings->iHac );
            break;
            }
        default:
            {
            User::Panic( KProfilePanicText, EProfilePanicInvalidSettingItemId );
            break;
            }
        }

    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::CreateSettingItemL(%d) end",aIdentifier );
    return item;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::SetObserver
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::SetObserver(
        MProfileExternalSettingsObserver& aObserver )
    {
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::SetObserver" );
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::AllowedToChange
// -----------------------------------------------------------------------------
//
TBool CProfilesGSTonesPluginSettingsHandler::AllowedToChange(
        TInt aSettingItemId ) const
    {
    switch( aSettingItemId )
        {
        case EProfileGSTonesPluginSettingCalendarToneId: // Fall through
        case EProfileGSTonesPluginSettingClockToneId:
            {
            return ETrue;
            }
        case EProfileGSTonesPluginSettingHacModeId:
            {
            return ( iFeatures->IsLocalFeatureSupported(
                     KTonesViewFeatureIdHac ) );
            }
        default:
            {
            return EFalse;
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::GetHelpContext
// -----------------------------------------------------------------------------
//
TBool CProfilesGSTonesPluginSettingsHandler::GetHelpContext(
        TCoeHelpContext& aContext ) const
    {
    aContext.iContext = KSET_HLP_PERSONAL_TONES;
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::SetHiddenItems
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::SetHiddenItems(
        CAknSettingItemArray& aSettingItemArray ) const
    {
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::SetHiddenItems" );
    TInt count( aSettingItemArray.Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        CAknSettingItem* item = aSettingItemArray.At( index );
        if( ( item->Identifier() == EProfileGSTonesPluginSettingHacModeId ) )
            {
            PRODEBUG( "CProfilesGSTonesPluginSettingHandler::SetHiddenItems hac" );
            item->SetHidden(
                !iFeatures->IsLocalFeatureSupported( KTonesViewFeatureIdHac ) );
            }
        }
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::SetHiddenItems end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::StoreIfChangedL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::StoreIfChangedL( TInt aSettingId )
    {
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::StoreIfChanged" );
    iTonesViewSettings->StoreIfChangedL( aSettingId );
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::StoreIfChanged end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::HandleNotifyString
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::HandleNotifyString( TUint32 aId,
        const TDesC16& /* aNewValue */ )
    {
    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::HandleNotifyString(%d) begin",aId );
    if( aId == KCalendarSoundFile )
        { // cannot leave - ignore return value:
        iCalendarSession->Get( KCalendarSoundFile, iCalendarTone );
        }
    else // Clock sound file
        { // cannot leave - ignore return value:
        iClockSession->Get( KClockAppSoundFile, iClockTone );
        }
    TRAP_IGNORE( iObserver->HandleExternalSettingsChangedL() );
    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::HandleNotifyString(%d) end",aId );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::StoreCalendarToneL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::StoreCalendarToneL(
        const TDesC& aTone )
    {
    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::StoreCalendarTone(%S)",&aTone );
    if( iCalendarTone.CompareF( aTone ) != 0 )
        {
        User::LeaveIfError(
                iCalendarSession->Set( KCalendarSoundFile, aTone ) );
        RemoveAutomatedContent( iCalendarTone );
        iCalendarTone.Copy( aTone );
        }
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::StoreClockToneL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::StoreClockToneL(
        const TDesC& aTone )
    {
    PRODEBUG1( "CProfilesGSTonesPluginSettingHandler::StoreClockTone(%S)",&aTone );
    if( iClockTone.CompareF( aTone ) != 0 )
        {
        User::LeaveIfError(
                iClockSession->Set( KClockAppSoundFile, aTone ) );
        RemoveAutomatedContent( iClockTone );
        iClockTone.Copy( aTone );
        }
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginCalendarDataInterface::
// CProfilesGSTonesPluginCalendarDataInterface
// -----------------------------------------------------------------------------
//
CProfilesGSTonesPluginCalendarDataInterface::
CProfilesGSTonesPluginCalendarDataInterface(
        CProfilesGSTonesPluginSettingsHandler& aSettingsHandler )
    : iSettingsHandler( aSettingsHandler )
    {}

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginCalendarDataInterface::ItemData
// -----------------------------------------------------------------------------
//
const TDesC& CProfilesGSTonesPluginCalendarDataInterface::ItemData()
    {
    return iSettingsHandler.iCalendarTone;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginCalendarDataInterface::SetItemDataL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginCalendarDataInterface::SetItemDataL(
        const TDesC& aDataBuf )
    {
    PRODEBUG( "CProfilesGSTonesPluginCalendarDataInterface::SetItemDataL" );
    iSettingsHandler.StoreCalendarToneL( aDataBuf );
    PRODEBUG( "CProfilesGSTonesPluginCalendarDataInterface::SetItemDataL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginClockDataInterface::
// CProfilesGSTonesPluginClockDataInterface
// -----------------------------------------------------------------------------
//
CProfilesGSTonesPluginClockDataInterface::
CProfilesGSTonesPluginClockDataInterface(
        CProfilesGSTonesPluginSettingsHandler& aSettingsHandler )
    : iSettingsHandler( aSettingsHandler )
    {}

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginClockDataInterface::ItemData
// -----------------------------------------------------------------------------
//
const TDesC& CProfilesGSTonesPluginClockDataInterface::ItemData()
    {
    return iSettingsHandler.iClockTone;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginClockDataInterface::SetItemDataL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginClockDataInterface::SetItemDataL(
        const TDesC& aDataBuf )
    {
    PRODEBUG( "CProfilesGSTonesPluginClockDataInterface::SetItemDataL" );
    iSettingsHandler.StoreClockToneL( aDataBuf );
    PRODEBUG( "CProfilesGSTonesPluginClockDataInterface::SetItemDataL end" );
    }

// End Of File

