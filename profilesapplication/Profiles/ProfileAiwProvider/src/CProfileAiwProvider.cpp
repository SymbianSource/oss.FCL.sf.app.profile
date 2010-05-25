/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile Aiw service provider.
*
*/


//	CLASS HEADER
#include "CProfileAiwProvider.h"

// INTERNAL INCLUDES
#include "CProfileEngineHandler.h"
#include "CPapPopupList.h"

//	EXTERNAL INCLUDES
#include <AiwMenu.h>	// For CAiwMenuPane
#include <AiwCommon.hrh> // For KAiwCmdAssign
#include <AiwCommon.h> // For MAiwNotifyCallback
#include <barsread.h>	// For TResourceReader
#include <profileaiwproviderresource.rsg> // For resource texts
#include <data_caging_path_literals.hrh>
#include <coemain.h> // CCoeEnv
#include <ConeResLoader.h> // RConeResourceLoader
#include <StringLoader.h>
#include <aknnotewrappers.h> // For CAknInformationNote
#include <aknlists.h>	// For CAknSingleGraphicPopupMenuStyleListBox
#include <cprofiletonehandler.h>
#include <MProfilesNamesArray.h>
#include <MProfileName.h>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <ProfileEngineDomainCRKeys.h>
#include <ProfileEng.hrh>	// For PROFILES_MAX_NAME_LENGTH
#include <eikfutil.h>	// For EikFileUtils
#include <centralrepository.h>
#include <featmgr.h>
#include <pathinfo.h>
#include <driveinfo.h>

#ifdef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM
#include <drmutility.h>
#include <drmagents.h>
#endif


//	CONSTANTS
namespace
	{
	_LIT( KProfileAiwProviderResourceFileName, "z:ProfileAiwProviderResource.rsc" );
	// Allowed MIME types
	_LIT( KAudioCommonMimeType, "audio/*");
	_LIT( KAudioRngMimeType,    "application/vnd.nokia.ringing-tone" );
#ifdef RD_VIDEO_AS_RINGING_TONE	
	_LIT( KVideoCommonMimeType, "video/*");
	_LIT( KRMVideoMimeType, "application/vnd.rn-realmedia" );
	_LIT( KRMVideoMimeType2, "application/x-pn-realmedia" );
	_LIT( KSDPVideoMimeType, "application/sdp" );
#endif	
	// Granularities
	const TInt KArrayGranularity( 3 );
	// List item format
	_LIT( KProfileAIWProviderIconAndTab, "1\t");
	// Maximum profile name length
	const TInt KProfileAiwProviderMaxNameLength( PROFILES_MAX_NAME_LENGTH );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileAiwProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileAiwProvider* CProfileAiwProvider::NewL()
    {
    CProfileAiwProvider* self = new(ELeave) CProfileAiwProvider;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::CProfileAiwProvider
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileAiwProvider::CProfileAiwProvider()
    {
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::ConstructL()
    {
    FeatureManager::InitializeLibL();
    TParse* fp = new(ELeave) TParse();
	fp->Set(KProfileAiwProviderResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL);
	TFileName resourceFileName( fp->FullName() );
	delete fp;

    // Get language of resource file
    BaflUtils::NearestLanguageFile( CCoeEnv::Static()->FsSession(), resourceFileName );
    iResourceOffset = CCoeEnv::Static()->AddResourceFileL( resourceFileName );
    iProfileToneHandler = CProfileToneHandler::NewL();
    // Move this to PopulateListBox()
    //iEngineHandler = CProfileEngineHandler::NewL();

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader,
                                               R_PROFILEAIWPROVIDER_MENU );
    // load strings from resource
	iWMAErrorText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_TEXT_NOT_ALLOWED );
	iMP4ErrorText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_INFO_TONE_DRM_PROTECTED );
	iUnprotectedErrorText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_INFO_TONE_NO_DRM );
	iNotSupportedErrorText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_INFO_MIME_TYPE_NOT_SUPPORTED );
	iPopupTitle = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_QUERY_SELECT_PROFILES );
	iChangedProfilesText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_CONFIRM_RINGTONE_CHANGED );
    iCopiedAndChangedText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_CONFIRM_RINGTONE_COPIED );

    // Read the maximum limit for the size of a tone file:
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
	CleanupStack::PushL( cenrep );
    User::LeaveIfError( cenrep->Get( KProEngRingingToneMaxSize, iMaxSize ) );
	CleanupStack::PopAndDestroy( cenrep );
	iSizeErrorText = StringLoader::LoadL(
        R_PROFILEAIWPROVIDER_TONE_MAXSIZE_ERROR, iMaxSize );

	CleanupStack::PopAndDestroy(); // reader
	}

// Destructor
CProfileAiwProvider::~CProfileAiwProvider()
    {
    delete iIndexToIdArray;
	delete iWMAErrorText;
	delete iMP4ErrorText;
	delete iUnprotectedErrorText;
	delete iNotSupportedErrorText;
	delete iPopupTitle;
	delete iChangedProfilesText;
	delete iSizeErrorText;
	delete iCopiedAndChangedText;

	delete iProfileToneHandler;
	delete iEngineHandler;
    CCoeEnv::Static()->DeleteResourceFile( iResourceOffset );
    FeatureManager::UnInitializeLib();
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::InitialiseL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::InitialiseL( MAiwNotifyCallback& /*aFrameworkCallback*/,
                                       const RCriteriaArray& /*aInterest*/ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::HandleServiceCmdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::HandleServiceCmdL( const TInt& aCmdId,
                                             const CAiwGenericParamList& aInParamList,
                                             CAiwGenericParamList& /*aOutParamList*/,
                                             TUint aCmdOptions,
                                             const MAiwNotifyCallback* aCallback )
    {
     // Cope with the design problems of AIW framework
    iConstNotifyCallback = const_cast<MAiwNotifyCallback*> (aCallback);

	if( aCmdOptions & KAiwOptCancel )
		{
		if( iPopup )
			{
			iPopup->CancelPopup();
		    iPopup = NULL;
			}
		return;
		}

  	if( aCmdId == KAiwCmdAssign )
    	{
        // Retrieve filename and file MIME type from AIW param list
		TPtrC fileName =
			GetAiwParamAsDescriptor(aInParamList, EGenericParamFile);

		TPtrC mimeTypeString =
			GetAiwParamAsDescriptor(aInParamList, EGenericParamMIMEType);

		// Leave, if there were not given all the required parameters
		if ((fileName == KNullDesC) || (mimeTypeString == KNullDesC))
			{
			return;
			}

		if( !CheckMimeTypeL( mimeTypeString ) )
			{
			// Not supported format
            ShowErrorNoteL( *iNotSupportedErrorText );
			}
		else if( IsFileWMDRMProtectedL( fileName ) )
			{
			ShowErrorNoteL( *iMP4ErrorText );
			}
        else if( !IsToneFileSizeOkL( fileName ) )
            {
			// Too big file
            ShowErrorNoteL( *iSizeErrorText );
            }
		else
			{
			LaunchL( *iPopupTitle, fileName );
			}
		}
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::PerformProfileChangeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::PerformProfileChangeL(
 const TDesC& aFileName, const TBool& aChangeAllProfiles ) const
	{
	TInt count( iSelectionArray->Count() );
	TInt err( KErrNone );
	TInt r( KErrNone );

	// Check location of the tone and copy if needed
	TFileName target;
	TBool remote = CopyIfRemoteL( aFileName, target );

	if( aChangeAllProfiles )
		{
		if( remote )
			{
			TRAP( r, err = iProfileToneHandler->SetToneForAllProfilesL(
		    	EProfileRingingToneSetting, target ) );
			}
		else
			{
			TRAP( r, err = iProfileToneHandler->SetToneForAllProfilesL(
	        	EProfileRingingToneSetting, aFileName ) );
			}
		}
	else
		{
        TInt profileId( 0 );
		for( TInt index(0); index < count; index++ )
			{
			profileId = iIndexToIdArray->At( iSelectionArray->At( index ) );
			
            if( remote )
                {
                // The first time set the tone, use SetProfileToneL to check the file
                if ( index == 0 )
                    {

                    TRAP( r, err = iProfileToneHandler->SetProfileToneL( 
                       profileId, EProfileRingingToneSetting, target ) );
                    }
                // the file has been checked at first time, use SetProfileToneL to
                // avoid repeat check.
                else
                    {
                    TRAP( r, err = iProfileToneHandler->SetProfileToneNotCheckL(
                        profileId, EProfileRingingToneSetting, target ) );
                    }
                }
            else
                {
                // The first time set the tone, use SetProfileToneL to check the file
                if ( index == 0 )
                    {
                    TRAP( r, err = iProfileToneHandler->SetProfileToneL( profileId,
                        EProfileRingingToneSetting, aFileName ) );
                    }
                // the file has been checked at first time, use SetProfileToneL to
                // avoid repeat check.
                else
                    {
                    TRAP( r, err = iProfileToneHandler->SetProfileToneNotCheckL( 
                        profileId, EProfileRingingToneSetting, aFileName ) );                    
                    }
                    				
                }

            // KErrNotFound is checked for the unlikely case in which the user
            // has first popped up the list of profiles and then switched to
            // Profiles App and deleted a profile or more, and finally completed
            // this "assigned to ringing tone" operation
            if( r == KErrNotFound )
                {
                r = KErrNone;
                }
            // The case r == KErrAccessDenied needn't be checked since only
            // those profiles that are modifiable are included in the list in
            // PopulateListBoxL() below
            if( r != KErrNone || err != KErrNone )
                {
                // no use to go on with other profiles if the tone can not be
                // assigned as an alert tone:
                break;
                }
			}
		}

	if( r == KErrPermissionDenied )
		{
		// Unprotected DRM tone case
        ShowErrorNoteL( *iUnprotectedErrorText );
		}
	else if( r == KErrArgument )
		{
		// WMA case
        ShowErrorNoteL( *iWMAErrorText );
		}
	else if( r != KErrNone )
		{
		User::Leave( r );
		}

	if ( err == KErrArgument )
    	{
    	// MP4 case
        ShowErrorNoteL( *iMP4ErrorText );
        }

	if( ( err == KErrNone ) && ( r == KErrNone )  && ( count > 0 ) )
		{
	   	CAknConfirmationNote* note = new( ELeave ) CAknConfirmationNote( EFalse );
	   	if( remote )
	   		{
	    	note->ExecuteLD( iCopiedAndChangedText->Des() );
	   		}
	   	else
	   		{
	    	note->ExecuteLD( iChangedProfilesText->Des() );
	   		}
   		}
	}

// -----------------------------------------------------------------------------
// CProfileAiwProvider::LaunchL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::LaunchL( const TDesC& aPopupTitle,
                                   const TDesC& aFileName )
	{
	// create listbox
 	// list item string format: "0\tLabel" where 0 is an index to icon array
    CEikFormattedCellListBox* listBox =
      new ( ELeave ) CAknSingleGraphicPopupMenuStyleListBox;
	CleanupStack::PushL( listBox );

    // create popup
    CPapPopupList* popup = CPapPopupList::NewL( listBox,
    	R_AVKON_SOFTKEYS_OK_CANCEL__UNMARK );
 	CleanupStack::PushL( popup );
 	popup->SetTitleL( aPopupTitle );

#ifdef RD_SCALABLE_UI_V2
	if( AknLayoutUtils::PenEnabled() )
		{
		listBox->ConstructL( popup, EAknListBoxStylusMultiselectionList );
		}
	else
		{
		listBox->ConstructL( popup, EAknListBoxMultiselectionList );
		}
#else
	listBox->ConstructL( popup, EAknListBoxMultiselectionList );
#endif

	listBox->CreateScrollBarFrameL(ETrue);
	listBox->ScrollBarFrame()->
		SetScrollBarVisibilityL(
		 CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

	popup->SetIconsL();

	// create model
    CDesCArrayFlat* items = new (ELeave)CDesCArrayFlat( KArrayGranularity );

	// populate model
	CleanupStack::PushL(items);
	CProfileAiwProvider::PopulateListBoxL( items );
	CleanupStack::Pop(items);

	CTextListBoxModel* model = listBox->Model();
    model->SetItemTextArray(items);

	// set all items as selected
	TInt count( listBox->Model()->NumberOfItems() );
	for( TInt index( 0 ); index < count; index++ )
		{
		listBox->View()->SelectItemL( index );
		}

	TBool changeAllProfiles( EFalse );

    // launch popup
    iPopup = popup;
    TInt res = popup->ExecuteLD();
   	// No leaving functions allowed between executeLD and CleanupStack::Pop().
    CleanupStack::Pop( popup );
    iPopup = NULL;

    if( res )
    	{
    	iSelectionArray = listBox->View()->SelectionIndexes();
    	if( count == iSelectionArray->Count() )
    		{
    		// User has selected to change all profiles
    		changeAllProfiles = ETrue;
    		}
	   	CProfileAiwProvider::PerformProfileChangeL( aFileName, changeAllProfiles );
    	}
    CleanupStack::PopAndDestroy(); // listBox
	}

// -----------------------------------------------------------------------------
// CProfileAiwProvider::PopulateListBoxL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::PopulateListBoxL( CDesCArrayFlat* aItems )
	{
	// iEngineHandler must be instantiated each time to make sure
	// profile list is up to date
	delete iEngineHandler;
	iEngineHandler = NULL;
    iEngineHandler = CProfileEngineHandler::NewL();
	
	MProfilesNamesArray* array = iEngineHandler->IdArray();
	MProfileEngineExtended* engine = iEngineHandler->Engine();

    // Create the mapping array so that the IDs can be mapped to Profile IDs
    // after the selection has been made
    // Delete it first in case we're called several times (in the same instance)
    delete iIndexToIdArray;
    iIndexToIdArray = NULL;
    iIndexToIdArray = new ( ELeave ) CArrayFixFlat<TInt>( array->MdcaCount() );

	// Add items
    TInt count( array->MdcaCount() );
	TBuf<PROFILES_MAX_NAME_LENGTH + 3> name; // + 3 for icon and tab characters
	for( TInt index( 0 ); index < count; index++ )
    	{
        const MProfileName* profileName = array->ProfileName( index );
        if( !profileName ) 
            {
            continue;
            }

        TInt profileId( profileName->Id() );
        // Only the profiles which modifiable flag of ringing tone item is
        // enabled should be included in the list. In Off-line profile case also
        // VoIP and WLAN must be supported (otherwise there wouldn't be need for
        // ringing tone in Off-line profile)
        if( ( profileId != EProfileOffLineId ) ||
            ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) &&
              FeatureManager::FeatureSupported( KFeatureIdProtocolWlan ) ) )
            {
            MProfileExtended* profile( engine->ProfileLC( profileId ) );
            TUint32 modFlags( profile->ModifiableFlags() );
            CleanupStack::PopAndDestroy(); // profile
            // Only the profiles which ringing tone item is modifiable should
            // be included in the list
            if( modFlags & EProfileFlagRingingTone )
                {
                name.Copy( KProfileAIWProviderIconAndTab );
                TBuf< KProfileAiwProviderMaxNameLength >
                    tmpName( array->MdcaPoint( index ) );
                AknTextUtils::ReplaceCharacters( tmpName,
                                                 KAknReplaceListControlChars,
                                                 TChar( ' ' ) );
                name.Append( tmpName );
                aItems->AppendL( name );
                iIndexToIdArray->AppendL( profileId );
                }
            }
        }
    iIndexToIdArray->Compress();
	}

// -----------------------------------------------------------------------------
// CProfileAiwProvider::InitializeMenuPaneL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::InitializeMenuPaneL( CAiwMenuPane& aMenuPane,
                                             TInt aIndex,
                                             TInt /*aCascadeId*/,
                                             const CAiwGenericParamList& aInParamList )
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader,
                                               R_PROFILEAIWPROVIDER_MENU );

 	// Retrieve filename and file MIME type from AIW param list
	TPtrC mimeTypeString = CProfileAiwProvider::GetAiwParamAsDescriptor
		(aInParamList, EGenericParamMIMEType);

	if( CProfileAiwProvider::CheckMimeTypeL( mimeTypeString ) )
		{
		// Only add menuitem if MIME type is valid
		aMenuPane.AddMenuItemsL( reader, KAiwCmdAssign, aIndex );
		}

    CleanupStack::PopAndDestroy(); // reader
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::HandleMenuCmdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::HandleMenuCmdL( TInt aMenuCmdId,
                                        const CAiwGenericParamList& aInParamList,
                                        CAiwGenericParamList& aOutParamList,
                                        TUint aCmdOptions,
                                        const MAiwNotifyCallback* aCallback )
    {
    // Route to HandleServiceCmdL
    HandleServiceCmdL(aMenuCmdId, aInParamList,
        aOutParamList, aCmdOptions, aCallback);
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::GetAiwParamAsDescriptor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CProfileAiwProvider::GetAiwParamAsDescriptor(
		const CAiwGenericParamList& aParamList,
		TGenericParamId aParamType)
	{
	TInt index = 0;
	const TAiwGenericParam* genericParam = NULL;
    genericParam = aParamList.FindFirst(
        index,
        aParamType,
        EVariantTypeDesC);

    if (index >= 0 && genericParam)
        {
        // Get the data
        return genericParam->Value().AsDes();
        }
	else
    	{
		return KNullDesC();
    	}
	}

// -----------------------------------------------------------------------------
// CProfileAiwProvider::CheckMimeTypeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileAiwProvider::CheckMimeTypeL(const TDesC& aMimeTypeString)
    {
    if( ( aMimeTypeString.MatchF( KAudioCommonMimeType ) == 0 ) ||
		( aMimeTypeString.CompareF( KAudioRngMimeType ) == 0 ) )
    	{
    	iIsVideo = EFalse;
    	return ETrue;
    	}

#ifdef RD_VIDEO_AS_RINGING_TONE	
    if( aMimeTypeString.MatchF( KVideoCommonMimeType ) == 0 ||
		aMimeTypeString.CompareF( KRMVideoMimeType ) == 0 ||
		aMimeTypeString.CompareF( KRMVideoMimeType2 ) == 0 ||
		aMimeTypeString.CompareF( KSDPVideoMimeType ) == 0 )
        {
        iIsVideo = ETrue;
		return ETrue;
        }
#endif

	return EFalse;
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::IsToneFileSizeOkL
// -----------------------------------------------------------------------------
//
TBool CProfileAiwProvider::IsToneFileSizeOkL( const TDesC& aFile )
    {
    if( !iMaxSize )
        { // no limit set -> every tone will be ok (what comes to size):
        return ETrue;
        }

    // Get file size
    TInt size = 0;
    TBool result( ETrue );

    TEntry entry;
    if ( CCoeEnv::Static()->FsSession().Entry( aFile, entry ) == KErrNone )
        {
        size = entry.iSize;        
        }

	// Check. NOTE: now if file size couldn't be determined, check fails.
	if ( size > (iMaxSize*KKilo) )
		{
        result = EFalse;
		}

    return result;
    }

// -----------------------------------------------------------------------------
// CProfileAiwProvider::ShowErrorNoteL
// -----------------------------------------------------------------------------
//
void CProfileAiwProvider::ShowErrorNoteL( const TDesC& aErrorText ) const
    {
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );
    note->ExecuteLD( aErrorText );
    }


// -----------------------------------------------------------------------------
// CProfileAiwProvider::CopyIfRemoteL
// -----------------------------------------------------------------------------
//
TBool CProfileAiwProvider::CopyIfRemoteL( const TDesC& aFileName, TFileName& aTarget ) const
    {
    TBool remote( EFalse );
	TParsePtrC parsedName( aFileName );
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
    TDriveUnit driveUnit( parsedName.Drive() );
	TUint driveStatus( 0 );

	DriveInfo::GetDriveStatus( fsSession, driveUnit, driveStatus );
	remote = ( ( driveStatus & DriveInfo::EDriveRemote ) == DriveInfo::EDriveRemote );

	if( remote )
		{
		// If the tone is on a remote drive, it must be copied
		aTarget.Insert( 0, parsedName.NameAndExt() );
		if( iIsVideo )
			{
			aTarget.Insert( 0, PathInfo::VideosPath() );
			}
		else
			{
			aTarget.Insert( 0, PathInfo::SoundsPath() );
			}
		aTarget.Insert( 0, PathInfo::PhoneMemoryRootPath() );
		User::LeaveIfError( EikFileUtils::CopyFile( aFileName, aTarget ) );
		}

	return remote;
    }


// -----------------------------------------------------------------------------
//
// Functions related to WMDRM protection check
//
// -----------------------------------------------------------------------------

#ifndef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM

// Some magic constants
static const TInt KMinContentLength( 16 );
//_LIT8( KContentProtectionType, "DRM" );
_LIT8( KASFHeaderObject, "75B22630668E11CFA6D900AA0062CE6C" );
_LIT8( KWrmHeader, "W\0R\0M\0H\0E\0A\0D\0E\0R\0" );


// -----------------------------------------------------------------------------
// FormatGUID
// -----------------------------------------------------------------------------
//
LOCAL_C void FormatGUID( TDes8& aGUID )
    {
    TBuf8<16> copyGUID( aGUID );
    TInt i;
    for( i = 0; i < 4; i++ )
        {
        copyGUID[i] = aGUID[3-i];
        }
    for( i = 4; i < 6; i++ )
        {
        copyGUID[i] = aGUID[9 - i];
        }
    for( i = 6; i < 8; i++ )
        {
        copyGUID[i] = aGUID[13 - i];
        }
    for( i = 8; i < 16 ; i++ )
        {
        copyGUID[i] = aGUID[i];
        }
    aGUID.Delete( 0, 32 );
    for( i = 0; i <16; i++ )
        {
        aGUID.AppendNumFixedWidthUC( copyGUID[i], EHex, 2 );
        }
    }

// -----------------------------------------------------------------------------
// ConvertToInt64
// -----------------------------------------------------------------------------
//
LOCAL_C TInt64 ConvertToInt64( TDesC8& aDes )
    {
    TInt64 num = 0;
    TInt i;
    for( i = 7 ; i >= 0; i-- )
        {
        num <<= 8;
        num |= aDes[i];
        }
    return num;
    }


// -----------------------------------------------------------------------------
// IsProtectedWmDrmL
// returns ETrue, if file is protected WMDRM file
//         EFalse if file is not protected WMDRM file
// Leaves with KErrUnderflow if file has too little data to decide
//         whether WmDrm or not
//         may also leave with other system wide error code
// -----------------------------------------------------------------------------
//
LOCAL_C TBool IsProtectedWmDrmL( RFile& aFileHandle )
    {
    TInt r( KErrNone );
    HBufC8* buffer( NULL );
    TInt pos( 0 );
    RFile file;
    TBuf8< 32 > header;

    TInt64 headerSize( 0 );
    TBool isProtectedWmDrm( EFalse );
    TPtr8 headerPtr( NULL, 0 );

    // Leave if given handle is invalid
    if( !aFileHandle.SubSessionHandle() )
        {
        User::Leave( KErrBadHandle );
        }

    User::LeaveIfError( file.Duplicate( aFileHandle ) );
    CleanupClosePushL( file );

    User::LeaveIfError( file.Seek( ESeekStart, pos ) );

    // Check if the file is an ASF file
    // : Check on runtime wether WM DRM is supporeted or not

    User::LeaveIfError( file.Read( 0, header, KMinContentLength ) );
    if( header.Length() < KMinContentLength )
        {
        User::Leave( KErrUnderflow );
        }

    FormatGUID( header );

    if( header == KASFHeaderObject )
        {
        // It's ASF, check still whether it's WM DRM protected or not
        file.Read( header,8 );
        headerSize = ConvertToInt64( header );
        if( headerSize <= 30 )
            {
            User::Leave( KErrUnderflow );
            }
        if ( headerSize > ( ( KMaxTInt32 / 2 ) - 1 ) )
            {
            User::Leave( KErrOverflow );
            }
        buffer = HBufC8::NewLC( headerSize );

        headerPtr.Set( buffer->Des() );
        User::LeaveIfError( file.Read( headerPtr, headerSize - 24 ) );

        r = headerPtr.Find( KWrmHeader );
        if ( KErrNotFound != r )
            {
            isProtectedWmDrm = ETrue;
            }
        CleanupStack::PopAndDestroy( buffer ); // buffer
        }
        CleanupStack::PopAndDestroy(); // file

    return isProtectedWmDrm;
    }

#endif // RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM

// -----------------------------------------------------------------------------
// CProfileAiwProvider::IsFileWMDRMProtectedL
// -----------------------------------------------------------------------------
//
TBool CProfileAiwProvider::IsFileWMDRMProtectedL( const TDesC& aFileName ) const
	{
	TBool res = EFalse;
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
	RFile hFile;

	TInt err = hFile.Open( fsSession, aFileName, 
						EFileRead | EFileStream | EFileShareReadersOnly );
	if( err == KErrInUse )
		{
		err = hFile.Open( fsSession, aFileName, 
						EFileRead | EFileStream | EFileShareAny );
		}
	if( err != KErrNone )
		{
		User::Leave( err );
		}
	CleanupClosePushL( hFile );

#ifdef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM
	TPtrC agent( KNullDesC );
	DRM::CDrmUtility* drmUtil( DRM::CDrmUtility::NewLC() );
	drmUtil->GetAgentL( hFile, agent );
	if( agent.Compare( DRM::KDrmWMAgentName ) == 0 )
		{
		res = ETrue;
		}
	CleanupStack::PopAndDestroy( drmUtil );
#else
	res = IsProtectedWmDrmL( hFile );
#endif

	CleanupStack::PopAndDestroy( &hFile );
	return res;
	}


// -----------------------------------------------------------------------------
//
// End of Functions related to WMDRM protection check
//
// -----------------------------------------------------------------------------


// End of File
