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
* Description:  Setting item class for tone selection.
*
*/



// CLASS HEADER
#include "CProfilesGSTonesPluginFileListSettingItem.h"

// INTERNAL INCLUDES
#include <Profile.hrh> // From Profile Engine
#include <ProfileSettingsView.hrh>
#include "ProfilesGSTonesPlugin.hrh"
#include <profilesgstonespluginrsc.rsg>
#include "MProfilesGSTonesPluginItemDataInterface.h"
#include "ProfilesDebug.h"

// SYSTEM INCLUDES
#include <centralrepository.h>
#include <coemain.h>
#include <CFLDFileListContainer.h>
#include <stringloader.h>
#include <data_caging_path_literals.hrh>
#include <ProfileEngineInternalCRKeys.h>
#include <HWRMVibraSDKCRKeys.h>
#include <CLFContentListing.hrh>

namespace
	{
// CONSTANTS
	_LIT( KIgnoredExtension, ".RNG" );
	_LIT( KProfileSilentTone, "Z:No_Sound.wav" );
    // MIME types that should not be shown by FileList
    _LIT( KProfileTypeMpegUrl, "audio/mpegurl" );
    _LIT( KProfileTypeRaPlugin, "audio/x-pn-realaudio-plugin" );
#ifndef __WMA
    _LIT( KProfileTypeWma, "audio/x-ms-wma" );
#endif
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::CProfilesGSTonesPluginFileListSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfilesGSTonesPluginFileListSettingItem::CProfilesGSTonesPluginFileListSettingItem(
    TInt aIdentifier,
    const TDesC& aDefaultToneFileName,
    CCoeEnv* aCoeEnv,
    MProfilesGSTonesPluginItemDataInterface& aDataInterface )
    :   CAknSettingItem( aIdentifier ),
        iDefaultToneFileName( aDefaultToneFileName ),
        iCoeEnv( aCoeEnv ),
        iDataInterface( aDataInterface )
    {
    }

// Destructor
CProfilesGSTonesPluginFileListSettingItem::~CProfilesGSTonesPluginFileListSettingItem()
    {
    PRODEBUG( "~CProfilesGSTonesPluginFileListSettingItem" );
	if( iVibraNotifyHandler )
		{
		iVibraNotifyHandler->StopListening();
		}

	if( iTypeNotifyHandler )
		{
		iTypeNotifyHandler->StopListening();
		}

	if( iVolumeNotifyHandler )
		{
		iVolumeNotifyHandler->StopListening();
		}

    delete iVibraNotifyHandler;
	delete iTypeNotifyHandler;
    delete iVolumeNotifyHandler;

    delete iSession;
    delete iVibraSession;

    delete iTitle;
    delete iNoSoundFileName;
    delete iNoSoundText;

    delete iDefaultToneText;
    delete iFileListContainer;
    PRODEBUG( "~CProfilesGSTonesPluginFileListSettingItem end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::CompleteConstructionL()
    {
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::CompleteConstr begin" );
    iNoSoundText =
		StringLoader::LoadL( R_PROFILE_TEXT_NONE );

	TFileName fileName;
	TParse* fp = new(ELeave) TParse();
	fp->Set(KProfileSilentTone, &KDC_RESOURCE_FILES_DIR, NULL);
	fileName.Copy( fp->FullName() );
	delete fp;
	iNoSoundFileName = fileName.AllocL();
    iTitle =
        StringLoader::LoadL( R_PROFILE_TEXT_SELECT_TONE );

    iVibraSession = CRepository::NewL(KCRUidVibraCtrl);
    iSession = CRepository::NewL(KCRUidProfileEngine);
    ReadMaxToneFileSizeL( *iSession );

	iVibraNotifyHandler = CCenRepNotifyHandler::NewL(*this,
            *iVibraSession,
            CCenRepNotifyHandler::EIntKey,
            KVibraCtrlProfileVibraEnabled);

    User::LeaveIfError( iVibraSession->Get( KVibraCtrlProfileVibraEnabled, iVibratingAlert ) );

    iVibraNotifyHandler->StartListeningL();

    if( Identifier() == EProfileGSTonesPluginSettingCalendarToneId )
        {
        iVolumeNotifyHandler = CCenRepNotifyHandler::NewL(*this,
                *iSession,
                CCenRepNotifyHandler::EIntKey,
                KProEngActiveRingingVolume);

        iTypeNotifyHandler = CCenRepNotifyHandler::NewL(*this,
                *iSession,
                CCenRepNotifyHandler::EIntKey,
                KProEngActiveRingingType);

        User::LeaveIfError( iSession->Get( KProEngActiveRingingVolume,
                                           iRingingVolume ) );
        User::LeaveIfError( iSession->Get( KProEngActiveRingingType,
                                           iRingingType ) );

        iVolumeNotifyHandler->StartListeningL();
        iTypeNotifyHandler->StartListeningL();
        }

    iDefaultToneText = StringLoader::LoadL(R_QTN_NO_PERSONAL_TONE, iCoeEnv);
    iInternalToneFileName.Copy( iDataInterface.ItemData() );
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::CompleteConstr end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::StoreL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::StoreL()
    {
    if( iInternalToneFileName.Length() == 0 )
        {
        iInternalToneFileName = *iNoSoundFileName; // No_Sound.wav
        }
    iDataInterface.SetItemDataL( iInternalToneFileName );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::LoadL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::LoadL()
    {
    iInternalToneFileName.Copy( iDataInterface.ItemData() );
    if( iInternalToneFileName.Length() == 0 )
        {
        iInternalToneFileName = *iNoSoundFileName;
        }
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::EditItemL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::EditItemL( TBool /* aCalledFromMenu */ )
    {
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL begin" );
    TInt ringingType( iRingingType );
    TInt volume( iRingingVolume );
    TInt id( Identifier() );
    if( id == EProfileGSTonesPluginSettingCalendarToneId )
        {
        if( ( ringingType != EProfileRingingTypeBeepOnce ) &&
            ( ringingType != EProfileRingingTypeSilent ) )
            {
            ringingType = EProfileRingingTypeRinging;
            }
        }
    else // Clock alert tone
        {
        ringingType = EProfileRingingTypeRinging;
        volume = EProfileRingingVolumeLevel7;
        }

    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL before FLD constr" );

    iFileListContainer = CFLDFileListContainer::NewL();
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL after FLD constr" );


    iFileListContainer->InsertNullItemL( *iDefaultToneText, iDefaultToneFileName );
    // Add "Off" as the last item in tone selection list.
    iFileListContainer->InsertEndNullItemL( *iNoSoundText, *iNoSoundFileName );
    iFileListContainer->SetRingingType( ringingType );
    iFileListContainer->SetVolume( volume );
    iFileListContainer->SetVibra( iVibratingAlert );
    iFileListContainer->AddExclusiveMimeTypeL( KProfileTypeMpegUrl );
    iFileListContainer->AddExclusiveMimeTypeL( KProfileTypeRaPlugin );
#ifndef __WMA
    iFileListContainer->AddExclusiveMimeTypeL( KProfileTypeWma );
#endif
    iFileListContainer->AddExclusiveMediaTypeL( ECLFMediaTypeVideo );

    if( id == EProfileGSTonesPluginSettingCalendarToneId )
        {
        iFileListContainer->SetAutomatedType(
                CDRMHelper::EAutomatedTypeCalendarAlarm );
        }
    else
        {
        iFileListContainer->SetAutomatedType(
                CDRMHelper::EAutomatedTypeClockAlarm );
        }

    // Size limit for voice call tone files
    if ( ( id == EProfileSettingRingingToneId      ||
           id == EProfileSettingRingingToneLine1Id ||
           id == EProfileSettingRingingToneLine2Id ) && iSizeLimit )
        {
        iFileListContainer->SetMaxFileSize( iSizeLimit );
        }
		
    // Launch File List Dialog
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL before FLD launch" );
    TBool returnValue( iFileListContainer->LaunchL( iInternalToneFileName, *iTitle ) );
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL after FLD launch" );
    if( returnValue )
        {
        // Setting Oked.
        iDataInterface.SetItemDataL( iInternalToneFileName ); 
        UpdateListBoxTextL();
        }

    delete iFileListContainer;
    iFileListContainer = NULL;
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::EditItemL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::SettingTextL
// -----------------------------------------------------------------------------
//
const TDesC& CProfilesGSTonesPluginFileListSettingItem::SettingTextL()
    {
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::SettingTextL begin" );
    if( iInternalToneFileName.Length() > 0 )
        {
        // FLD has returned a filename
        if( iInternalToneFileName.CompareF( *iNoSoundFileName ) == 0 )
            {
            // Filename is "No_Sound", return localized text "None"
            iSettingText.Set( *iNoSoundText );
            }
        else if( iInternalToneFileName.CompareF( iDefaultToneFileName ) == 0 )
            {
            // Filename is "No_Sound", return localized text "None"
            iSettingText.Set( *iDefaultToneText );
            }
        else
            {
            // Filename is something else
            TParsePtrC parse( iInternalToneFileName );
            if( parse.Ext().CompareF( KIgnoredExtension ) == 0 )
                {
                iSettingText.Set( parse.Name() );
                }
            else
                {
                iSettingText.Set( parse.NameAndExt() );
                }
            }
        }
    else
        {
        // Filename is an empty descriptor, return localised text "None"
        iSettingText.Set( *iNoSoundText );
        }
    PRODEBUG( "CProfilesGSTonesPluginFileListSettingItem::SettingTextL end" );
    return iSettingText;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginFileListSettingItem::HandleNotifyInt
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::HandleNotifyInt( TUint32 aId,
        TInt aNewValue )
    {
    switch( aId )
        {
        case KVibraCtrlProfileVibraEnabled:
            {
            iVibratingAlert = aNewValue;
            if( iFileListContainer )
                {
                iFileListContainer->SetVibra( iVibratingAlert );
                }
            break;
            }
        case KProEngActiveRingingType:
            {
            iRingingType = aNewValue;
            if( iFileListContainer )
                {
                iFileListContainer->SetRingingType( iRingingType );
                }
            break;
            }
        default: // KProEngActiveRingingVolume 
            {
            iRingingVolume = aNewValue;
            if( iFileListContainer )
                {
                iFileListContainer->SetVolume( iRingingVolume );
                }
            break;
            }
        }

    }

// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::ReadMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginFileListSettingItem::ReadMaxToneFileSizeL(
        CRepository& aRepository )
    {
    // Set file size limit if configured ON.
	User::LeaveIfError( aRepository.Get( KProEngRingingToneMaxSize,
                                         iSizeLimit ) );
	if ( iSizeLimit < 0 )
	    {
	    iSizeLimit = 0;
	    }
	iSizeLimit *= KKilo;
    }

// End Of File
