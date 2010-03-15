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
* Description:  A helper interface to be used by S60 applications for
*                setting ringing and alert tones for different profiles.
*                Registers DRM protected tones as automated content
*                and removes old content from automated content list.
*                Shows a content activation query if necessary.
*
*/



// CLASS HEADER
#include    "cprofiletonehandler.h"

// INTERNAL INCLUDES
#include	"ProfileMmfInfoUtility.h"

// EXTERNAL INCLUDES
#include 	<centralrepository.h>
#include    <ProfileEngineDomainCRKeys.h> // KProEngRingingToneMaxSize
#include    <bautils.h> // BaflUtils
#include    <apgcli.h> // RApaLsSession
#include    <coemain.h> // CCoeEnv
#include    <aknnotewrappers.h> // CAknInformationNote
#include    <ConeResLoader.h> // RConeResourceLoader
#include	<StringLoader.h>
#include    <MProfileEngineExtended.h>
#include    <MProfilesLocalFeatures.h>
#include    <MProfileExtended.h>
#include    <MProfileSetExtraTones.h>
#include    <MProfileSetTones.h>
#include    <MProfileExtraTones.h>
#include    <MProfileTones.h>
#include    <MProfilesNamesArray.h>
#include    <MProfileName.h>
#include    <MProfileUtilitySingleton.h>
#include    <ProfileSettingsView.rsg>
#include    <AknGlobalNote.h>
#include    <f32file.h>

namespace
	{
	// CONSTANTS
	_LIT( KProfileSilentTone,     "Z:No_Sound.wav" );
	_LIT( KProfileAudioMimeType,  "audio" );
	_LIT( KProfileRngMimeType,    "application/vnd.nokia.ringing-tone" );

	_LIT( KProfileCommonVideoType, "video/*" );
	_LIT( KProfileRMVideoMimeType, "application/vnd.rn-realmedia" );
	_LIT( KProfileRMVideoMimeType2, "application/x-pn-realmedia" );
	_LIT( KProfileSDPVideoMimeType, "application/sdp" );
	_LIT( KProfilePlainTextType, "text/plain" );
	// The filename of the resource file
	_LIT( KProfileResourceFileName, "Z:ProfileSettingsView.RSC" );
	}



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileToneHandler::CProfileToneHandler
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileToneHandler::CProfileToneHandler()
    {
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::ConstructL()
    {
    DrmConstructL();
    GetMaxToneFileSizeL();
    User::LeaveIfError( iFs.Connect() );
    iProfileEngine = CreateProfileEngineExtendedL( &iFs );
    // NB. Singleton must be released in the destructor:
    iProfilesFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );
        
    User::LeaveIfError( iSSSettings.Open() );
    iSSSettings.Get( ESSSettingsAls, iAlternateLineSetting ); // ignore errors
    iSSSettings.Register( ESSSettingsAls, *this ); // ignore errors
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileToneHandler* CProfileToneHandler::NewLC()
    {
    CProfileToneHandler* self = new( ELeave ) CProfileToneHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CProfileToneHandler* CProfileToneHandler::NewL()
    {
    CProfileToneHandler* self = CProfileToneHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProfileToneHandler::~CProfileToneHandler()
    {
    iSSSettings.Close();
    iFs.Close();
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
        
    if ( iProfilesFeatures )
        {
        ReleaseProfileUtility();    
        }
    
    ReleaseDrm();
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetActiveProfileRingingToneL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileToneHandler::SetActiveProfileRingingToneL(
    const TDesC& aFileName )
    {
    // Find out the ID of the active profile
    TInt activeProfileId(
        User::LeaveIfError( iProfileEngine->ActiveProfileId() ) );

    return SetProfileToneL(
        activeProfileId, EProfileRingingToneSetting, aFileName );
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetProfileToneL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileToneHandler::SetProfileToneL( TInt aProfileId,
    TProfileTones aToneSetting, const TDesC& aFileName )
    {
    // Check the file which is tried to set as ringing or alert tone.
    TInt returnValue( CheckFileL( aFileName ) );
    returnValue = CheckResultAndProtectionL( returnValue, aFileName );
    if( returnValue != KErrNone )
        {
        return returnValue;
        }

    DoSetProfileToneL( aProfileId, aToneSetting, aFileName );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetProfileToneNotCheckL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileToneHandler::SetProfileToneNotCheckL( TInt aProfileId,
                TProfileTones aToneSetting, const TDesC& aFileName )
    {
    DoSetProfileToneL( aProfileId, aToneSetting, aFileName );
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetToneForAllProfilesL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CProfileToneHandler::SetToneForAllProfilesL(
    TProfileTones aToneSetting, const TDesC& aFileName )
    {
    // Check the file which is tried to set as ringing or alert tone.
    TInt returnValue( CheckFileL( aFileName ) );
    returnValue = CheckResultAndProtectionL( returnValue, aFileName );
    if( returnValue != KErrNone )
        {
        return returnValue;
        }

    const MProfilesNamesArray* idArray = iProfileEngine->ProfilesNamesArrayLC();
    TInt arrayCount( idArray->MdcaCount() );
    TInt arrayIndex( 0 );
    TInt profileId( 0 );
    TInt activeId( iProfileEngine->ActiveProfileId() );

    // Go through profiles and update tone
    while( arrayIndex < arrayCount )
        {
        profileId = idArray->ProfileName( arrayIndex )->Id();
        // Updating of the active profile will be done last to make sure that
        // UI gets the new value of the ringing tone if settings view is active
        // and it is containing settings of another than active profile
        if( profileId != activeId )
            {
            TRAPD(r, DoSetProfileToneL( profileId, aToneSetting, aFileName ));
            // Filter situation when profile is not allowed to be changed
            // e.g. Drive-profile
            if( ( r != KErrNone ) && ( r != KErrAccessDenied ) )
                {
                User::Leave( r );
                }
            }
        arrayIndex++;
        }
    CleanupStack::PopAndDestroy(); // idArray

    // Update the active profile:
    TRAPD(r, DoSetProfileToneL( activeId, aToneSetting, aFileName ));
    // Filter situation when profile is not allowed to be changed
    // e.g. Drive-profile
    if( ( r != KErrNone ) && ( r != KErrAccessDenied ) )
        {
        User::Leave( r );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::PhoneSettingChanged
// Callback method inherited from MSSSettingsObserver for getting events if
// Alternate Line Service status changes.
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::PhoneSettingChanged(
    TSSSettingsSetting aSetting, TInt aNewValue )
    {
    if( aSetting == ESSSettingsAls )
        {
        iAlternateLineSetting = aNewValue;
        }
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::DoSetProfileToneL
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::DoSetProfileToneL( TInt aProfileId,
    TProfileTones aToneSetting, const TDesC& aFileName )
    {
    // Read the settings of the profile in question
    MProfileExtended* profile = iProfileEngine->ProfileLC( aProfileId );

    // Get the old tone in order to remove it from the automated content list
    TFileName oldTone( ReadToneL( *profile, aToneSetting ) );

	TFileName fileName( aFileName );
	if( fileName.Length() == 0 )
        {
        // If no filename is given, replace it with "No_sound.wav". This way
        // it's possible to e.g. play vibra during incoming call.
		TParse* fp = new(ELeave) TParse();
		fp->Set(KProfileSilentTone, &KDC_RESOURCE_FILES_DIR, NULL);
		fileName.Copy( fp->FullName() );
		delete fp;
        }

    // Replace the old tone with the new tone
    SetToneL( *profile, aToneSetting, fileName );

    // Commit changes. Write the new settings to the Profiles Engine
    iProfileEngine->CommitChangeL( *profile );

    // Register file as automated content
    TInt err( SetAutomated( aToneSetting, fileName ) );
    if( err == KErrNone )
        {
        // Remove old tone from the automated content list
        RemoveAutomated( oldTone );
        }
    else
        {
        // Put the old tone back into Profiles:
        TRAP_IGNORE(
                SetToneL( *profile, aToneSetting, oldTone );
                iProfileEngine->CommitChangeL( *profile );
                );
        }

    CleanupStack::PopAndDestroy(); // profile
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetToneL
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::SetToneL( MProfileExtended& aProfile,
    TProfileTones aToneSetting, const TDesC& aFileName )
    {
    MProfileSetExtraTones& setExtraTones = aProfile.ProfileSetExtraTones();
    MProfileSetTones& setTones = aProfile.ProfileSetTones();

    switch( aToneSetting )
        {
        case EProfileRingingToneSetting:
            {
            if( iAlternateLineSetting == ESSSettingsAlsAlternate )
                {
                // Alternate Line Service is supported and alternate line is
                // selected. Set as ringing tone for line 2.
                setTones.SetRingingTone2L( aFileName );
                }
            else
                {
                // Alternate Line Service is not supported or primary line is
                // selected. Set as ringing tone for line 1.
                setTones.SetRingingTone1L( aFileName );
                }
            break;
            }
        case EProfileVideoCallToneSetting:
            {
            setExtraTones.SetVideoCallRingingToneL( aFileName );
            break;
            }
        case EProfileMessageAlertToneSetting:
            {
            setTones.SetMessageAlertToneL( aFileName );
            break;
            }
        case EProfileInstantMessageAlertToneSetting:
            {
            setExtraTones.SetIMMessageAlertToneL( aFileName );
            break;
            }
        case EProfileEmailAlertToneSetting:
            {
            setExtraTones.SetEmailAlertToneL( aFileName );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ReadToneL
// -----------------------------------------------------------------------------
//
const TDesC& CProfileToneHandler::ReadToneL( const MProfileExtended& aProfile,
    TProfileTones aToneSetting ) const
    {
    const MProfileExtraTones& extraTones = aProfile.ProfileExtraTones();
    const MProfileTones& tones = aProfile.ProfileTones();

    switch( aToneSetting )
        {
        case EProfileRingingToneSetting:
            {
            if( iAlternateLineSetting == ESSSettingsAlsAlternate )
                {
                // Alternate Line Service is supported and alternate line is
                // selected.
                return tones.RingingTone2();
                }
            else
                {
                // Alternate Line Service is not supported or primary line is
                // selected.
                return tones.RingingTone1();
                }
            }
        case EProfileVideoCallToneSetting:
            {
            return extraTones.VideoCallRingingTone();
            }
        case EProfileMessageAlertToneSetting:
            {
            return tones.MessageAlertTone();
            }
        case EProfileInstantMessageAlertToneSetting:
            {
            return extraTones.IMMessageAlertTone();
            }
        case EProfileEmailAlertToneSetting:
            {
            return extraTones.EmailAlertTone();
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CheckFileL
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::CheckFileL( const TDesC& aFileName )
    {
    if( aFileName.Length() == 0 )
        {
        // The filename is empty, ok.
        // It will be changed to "No_sound.wav" when writing to Profiles Engine.
        return KErrNone;
        }

    if( !BaflUtils::FileExists( iFs, aFileName ) )
        {
        // The file does not exist. No good (start the dance).
        return KErrNotFound;
        }

   	TBool isAudio( EFalse );
	TBool isVideo( EFalse );

    // The file exists. Check its data (MIME) type.
    TBuf<KMaxDataTypeLength> dataType( DataTypeL( aFileName ) );
    if( ( dataType.Left( KProfileAudioMimeType()
    .Length() ).CompareF( KProfileAudioMimeType ) == 0 ) ||
		( dataType.CompareF( KProfileRngMimeType ) == 0 ) )
    	{
    	isAudio = ETrue;
    	}

    if( dataType.MatchF( KProfileCommonVideoType ) == 0 ||
		dataType.CompareF( KProfileRMVideoMimeType ) == 0 ||
		dataType.CompareF( KProfileRMVideoMimeType2 ) == 0 ||
		dataType.CompareF( KProfileSDPVideoMimeType ) == 0 )
        {
#ifdef RD_VIDEO_AS_RINGING_TONE	
        isVideo = ETrue;
#else
        isVideo = EFalse;
#endif
        }

	if( (!isAudio) && (!isVideo) && (dataType.CompareF(KProfilePlainTextType) != 0) )
		{
	    // The data type does not start with "audio" or video and it's not RNG file.
    	return KErrNotSupported;
		}
		
	// Check whether file is supported by MMF
    TBuf8<KMaxDataTypeLength> tempDataType;
    tempDataType.Copy( dataType );

    if( !ProfileMmfInfoUtility::IsMimeTypeSupportedL( tempDataType ) )
    	{
    	// File is not supported by MMF
    	return KErrNotSupported;
    	}

	// Operator requirement. Check if the tones of this data type are blocked:
    if( iProfilesFeatures->IsBlockedType( dataType ) ||
        iProfilesFeatures->IsExcludedType( dataType ) )
        {
        User::Leave( KErrArgument );
        }

	// Check if the tone is WMDRM protected
	if( IsFileWMDRMProtectedL( aFileName ) )
		{
		return KErrPermissionDenied;
		}

    TInt result( KErrNone );
    if( IsProtected( aFileName ) )
        {
        // In certain variants tones of certain MIME-types are not allowed as
        // alert tones if they are DRM-protected, check if this is one:
        if( iProfilesFeatures->IsBlockedProtectedType( dataType ) )
            {
            return KErrArgument;
            }
        result = CheckProtectedFileL( aFileName );
        }
    else
        {
        result = iProfilesFeatures->IsBlockedUnprotectedType( dataType ) ?
                 KErrPermissionDenied : KErrNone;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CheckResultAndProtectionL
// Find out the mime type of the file
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::CheckResultAndProtectionL( TInt aResult,
    const TDesC& aFileName )
    {
    if( aResult == KErrPermissionDenied || aResult == KErrNotSupported ||
        aResult == KErrNotFound )
        {
        User::Leave( aResult );
        }
    else if( aResult != KErrNone )
        {
        return aResult;
        }

    // If tone file size is limited,
    // check if this file's size exceeds this limit.
    if ( iMaxSizeKB )
        {
        if ( CheckToneFileSizeL( aFileName, iMaxSizeKB ) != KErrNone )
            {
            ShowSizeErrorNoteL( iMaxSizeKB );
            return KErrTooBig;
            }
        }

    if( IsProtected( aFileName ) )
        {
        if( !CanSetAutomated( aFileName ) )
            {
            // DRM v2 content should be caught here
            ShowErrorNoteL( R_PROFILE_TEXT_DRM_PREV_RIGHTS_SET );
            return KErrGeneral;
            }

        // If the file in question is unactivated protected content,
        // ask user if he/she wants to activate it.
        if( AskAutomated( aFileName ) == KErrCancel )
            {
            // User doesn't want to activate unactivated protected content.
            return KErrCancel;
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::DataType
// Find out the mime type of the file
// -----------------------------------------------------------------------------
//
TBuf<KMaxDataTypeLength> CProfileToneHandler::DataTypeL(
    const TDesC& aFileName ) const
    {
    RApaLsSession apaLsSession;
    User::LeaveIfError( apaLsSession.Connect() );
    CleanupClosePushL( apaLsSession );

    TUid dummyUid = { 0 }; // instantiate as zero
    TDataType dataType( dummyUid );
    User::LeaveIfError(
        apaLsSession.AppForDocument( aFileName, dummyUid, dataType ) );

    CleanupStack::PopAndDestroy(); // apaLsSession.Close()
    return dataType.Des();
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ShowErrorNoteL
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::ShowErrorNoteL( TInt aResourceId ) const
    {
    CCoeEnv* coeEnv = CCoeEnv::Static();
    RConeResourceLoader resourceLoader( *coeEnv );
	CleanupClosePushL( resourceLoader );

	TParse* fp = new(ELeave) TParse();
	fp->Set(KProfileResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL);
	TFileName localizedFileName( fp->FullName() );
	delete fp;

    resourceLoader.OpenL( localizedFileName );

    HBufC* errorText = StringLoader::LoadLC( aResourceId );
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );
    note->ExecuteLD( *errorText );

	// errorText
    CleanupStack::PopAndDestroy( errorText );
	// resourceLoader
	CleanupStack::PopAndDestroy( );
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ShowSizeErrorNoteL
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::ShowSizeErrorNoteL( TInt aSizeLimitKB )
    {
    CCoeEnv* coeEnv = CCoeEnv::Static();
    RConeResourceLoader resourceLoader( *coeEnv );
	CleanupClosePushL( resourceLoader );

	TParse* fp = new(ELeave) TParse();
	CleanupStack::PushL( fp );
	fp->Set( KProfileResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL);
	
	TFileName* fileName = new( ELeave ) TFileName();
	fileName->Copy( fp->FullName() );
	CleanupStack::PushL( fileName );
    resourceLoader.OpenL( *fileName );
    CleanupStack::PopAndDestroy( fileName );

    HBufC* errorText = StringLoader::LoadLC(  R_PROFILE_TEXT_TONE_MAXSIZE_ERROR, aSizeLimitKB );
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );
    note->ExecuteLD( *errorText );

    CleanupStack::PopAndDestroy( errorText );
    CleanupStack::PopAndDestroy( fp );
	CleanupStack::PopAndDestroy( &resourceLoader );
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::GetMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::GetMaxToneFileSizeL()
    {
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
	CleanupStack::PushL( cenrep );
	TInt error = cenrep->Get( KProEngRingingToneMaxSize, iMaxSizeKB );
	CleanupStack::PopAndDestroy( cenrep );
	if ( error != KErrNone )
    	{
        iMaxSizeKB = 0;
    	}
	if ( iMaxSizeKB < 0 )
	    {
	     iMaxSizeKB = 0;
	    }
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CheckToneFileSizeL
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::CheckToneFileSizeL( const TDesC& aFile,
                                              TInt aSizeLimitKB )
    {
    // Get file size
    TInt size = 0;
    TInt error = KErrNone;

    TEntry entry;
    if ( iFs.Entry( aFile, entry ) == KErrNone )
        {
        size = entry.iSize;        
        }

	// Check. NOTE: now if file size couldn't be determined, check fails.
	aSizeLimitKB *= KKilo;
	if ( aSizeLimitKB  &&  size > aSizeLimitKB )
		{
		error = KErrTooBig;
		}

    return error;
    }


//  End of File
