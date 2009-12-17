/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "CProfileFileListSettingItem.h"

// INTERNAL INCLUDES
#include <coemain.h>

#include <profile.hrh> // From Profile Engine
#include <StringLoader.h>
#include <centralrepository.h>
#include <profileengineinternalcrkeys.h>
#include <ProfileSettingsView.rsg>
#include <data_caging_path_literals.hrh>
#include <CLFContentListing.hrh>	// For TCLFMediaType::ECLFMediaTypeVideo

// for mediafilelist
#include <mediafilelist.h>
#include <profileinternal.hrh>
#include <DRMHelper.h>


// EXTERNAL INCLUDES
#include "ProfilePanics.h"
#include "ProfileSettingsView.hrh"

namespace
	{
// CONSTANTS
	_LIT( KIgnoredExtension, ".RNG" );
	_LIT( KProfileSilentTone, "Z:No_Sound.wav" );

	_LIT( KProfileAudioMpeg, "audio/mpegurl" );
	_LIT( KProfileAudioRAPlugin, "audio/x-pn-realaudio-plugin" );
#ifndef __WMA
	_LIT( KProfileAudioWMA, "audio/x-ms-wma" );
#endif
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::CProfileFileListSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileFileListSettingItem::CProfileFileListSettingItem(
    TInt aIdentifier,
    TDes& aToneFileName,
    const TInt& aRingingType,
    const TInt& aRingingVolume,
    const TBool& aVibratingAlert,
    TInt& a3DEffect,
    TInt& a3DEcho,
    CCoeEnv* aCoeEnv )
    :   CAknSettingItem( aIdentifier ),
        iExternalToneFileName( aToneFileName ),
        iRingingType( aRingingType ),
        iRingingVolume( aRingingVolume ),
        iVibratingAlert( aVibratingAlert ),
        i3DEffect( a3DEffect ),
        i3DEcho( a3DEcho ),
        iCoeEnv( aCoeEnv )
    {
    }

// Destructor
CProfileFileListSettingItem::~CProfileFileListSettingItem()
    {
    delete iTitle;
    delete iNoSoundFileName;
    delete iNoSoundText;
    delete iInternalToneFileName;
    }

// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfileFileListSettingItem::CompleteConstructionL()
    {
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

	iInternalToneFileName = new( ELeave ) TFileName();
    }

// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::StoreL
// -----------------------------------------------------------------------------
//
void CProfileFileListSettingItem::StoreL()
    {
    if( iInternalToneFileName->Length() == 0 )
        {
        iInternalToneFileName->Copy( iNoSoundFileName->Des() );
        }
    iExternalToneFileName.Copy( *iInternalToneFileName );
    }

// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::LoadL
// -----------------------------------------------------------------------------
//
void CProfileFileListSettingItem::LoadL()
    {
    iInternalToneFileName->Copy( iExternalToneFileName );
    
    if( iInternalToneFileName->Length() == 0 )
        {
        iInternalToneFileName->Copy( iNoSoundFileName->Des() );
        }
    }


// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::EditItemL
// -----------------------------------------------------------------------------
//
void CProfileFileListSettingItem::EditItemL( TBool /*aCalledFromMenu*/ )
    {
    TInt ringingType( iRingingType );
    TInt id( Identifier() );
    if( ( id == EProfileSettingMessageAlertToneId ) ||
        ( id == EProfileSettingEmailAlertToneId ) )
        {
        // Message alert tone is always played EProfileRingingTypeRingingOnce
        // Exceptions are Silent and Beeb Once
         if( ( ringingType == EProfileRingingTypeRinging ) ||
            ( ringingType == EProfileRingingTypeAscending ) )
            {
            ringingType = EProfileRingingTypeRingingOnce;
            }
        }

    TFileName fileName;
    TInt nullItem = KErrNotFound;    
        
    CMediaFileList* list = CMediaFileList::NewL();
    CleanupStack::PushL( list );

    list->SetAttrL( CMediaFileList::EAttrTitle, *iTitle );
    list->SetAttrL( CMediaFileList::EAttrVolume, iRingingVolume );
    list->SetAttrL( CMediaFileList::EAttrVibra, iVibratingAlert );
    list->SetAttrL( CMediaFileList::EAttrRingingType, ringingType );
    list->SetAttrL( CMediaFileList::EAttrExcludeMimeType, KProfileAudioMpeg );
    list->SetAttrL( CMediaFileList::EAttrExcludeMimeType, KProfileAudioRAPlugin );
#ifndef __WMA
    list->SetAttrL( CMediaFileList::EAttrExcludeMimeType, KProfileAudioWMA );
#endif
  
    list->SetAttrL( CMediaFileList::EAttrAutomatedType, CDRMHelper::EAutomatedTypeRingingTone );

    if( id == EProfileSettingPTTRingingToneId )
        {
        // PTT does not allow videos
        list->SetAttrL( CMediaFileList::EAttrExcludeFolder, CMediaFileList::EMediaFileTypeVideo );
        }

    if( id == EProfileSettingMessageAlertToneId )
        {
        list->SetAttrL( CMediaFileList::EAttrAutomatedType, CDRMHelper::EAutomatedTypeMessageAlert );
        }
    else if( id == EProfileSettingEmailAlertToneId )
        {
        list->SetAttrL( CMediaFileList::EAttrAutomatedType, CDRMHelper::EAutomatedTypeEmailAlert );
        }
       
    // Size limit for voice call tone files
    if ( id == EProfileSettingRingingToneId      ||
         id == EProfileSettingRingingToneLine1Id ||
         id == EProfileSettingRingingToneLine2Id )
        {
        TInt sizeLimitKB = 0;
        CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
        CleanupStack::PushL( cenrep );
        User::LeaveIfError( cenrep->Get( KProEngRingingToneMaxSize, sizeLimitKB ) );
        CleanupStack::PopAndDestroy(); // cenrep

        list->SetAttrL( CMediaFileList::EAttrFileSize, sizeLimitKB );
        

        }

    list->SetAttrL( CMediaFileList::EAttr3DEffect, i3DEffect );
    list->SetAttrL( CMediaFileList::EAttr3DEcho, i3DEcho );

    list->SetNullItemL( *iNoSoundText, *iNoSoundFileName,
                        CMediaFileList::EMediaFileTypeAudio,
                        CMediaFileList::ENullItemIconOff );
        
    TInt ret = list->ShowMediaFileListL( iInternalToneFileName, &nullItem, NULL, NULL );
     
    CleanupStack::PopAndDestroy( list );

    if( ret )
        {
        // Setting Oked.
        UpdateListBoxTextL();
        }
    }




// -----------------------------------------------------------------------------
// CProfileFileListSettingItem::SettingTextL
// -----------------------------------------------------------------------------
//
const TDesC& CProfileFileListSettingItem::SettingTextL()
    {
    if( iInternalToneFileName->Length() > 0 )
        {
        // FLD has returned a filename
        if( iInternalToneFileName->CompareF( *iNoSoundFileName ) == 0 )
            {
            // Filename is "No_Sound", return localized text "None"
            iSettingText.Set( *iNoSoundText );
            }
        else
            {
            // Filename is something else
            TParsePtrC parse( *iInternalToneFileName );
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
    return iSettingText;
    }

// End Of File
