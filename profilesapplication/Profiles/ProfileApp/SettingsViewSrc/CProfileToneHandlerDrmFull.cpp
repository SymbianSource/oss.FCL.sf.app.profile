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
* Description:  A helper interface to be used by S60 applications for
*                setting ringing and alert tones for different profiles.
*                Registers DRM protected tones as automated content
*                and removes old content from automated content list.
*                Shows a content activation query if necessary.
*
*/



// INCLUDE FILES
#include    "cprofiletonehandler.h"

#include    <DRMHelper.h>
#include    <DRMCommon.h>
#include    <ProfileSettingsView.rsg>

#ifdef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM
#include	<drmutility.h>
#include	<drmagents.h>
#endif

#include <caf/caf.h>


// ============================ LOCAL FUNCTIONS ================================

CDRMHelper::TDRMHelperAutomatedType ToneSettingToAutomatedType(
        TProfileTones aToneSetting )
    {
    switch( aToneSetting )
        {
        case EProfileRingingToneSetting:
            {
            return CDRMHelper::EAutomatedTypeRingingTone;
            }
        case EProfileMessageAlertToneSetting:
            {
            return CDRMHelper::EAutomatedTypeMessageAlert;
            }
        case EProfileEmailAlertToneSetting:
            {
            return CDRMHelper::EAutomatedTypeEmailAlert;
            }
        case EProfileInstantMessageAlertToneSetting:
            {
            return CDRMHelper::EAutomatedTypeIMAlert;
            }
        default:
            {
            return CDRMHelper::EAutomatedTypeOther;
            }
        }
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileToneHandler::DrmConstructL
// 2nd phase construct for DRM objects
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::DrmConstructL()
    {
    iDrmCommon = DRMCommon::NewL();
    iDrmHelper = CDRMHelper::NewL();
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ReleaseDrm
// Destructor for DRM objects
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::ReleaseDrm()
    {
    delete iDrmHelper;
    iDrmHelper = NULL;
    delete iDrmCommon;
    iDrmCommon = NULL;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CheckProtectedFileL
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::CheckProtectedFileL( const TDesC& aFileName )
    {
    DRMCommon::TContentProtection protection;
    HBufC8* mimeType = NULL; // ignored
    HBufC8* contentUri = NULL;
    TUint dataLength; // ignored
    TInt err = iDrmCommon->GetFileInfo( aFileName, protection, mimeType,
                                        contentUri, dataLength );
    delete mimeType;
    if( err )
        {
        delete contentUri;
        User::Leave( err );
        }
    CleanupStack::PushL( contentUri );

    TInt result( KErrGeneral );
    CDRMRights* rights = NULL;
    err = iDrmCommon->GetActiveRights( *contentUri, DRMCommon::EPlay, rights );

    CleanupStack::PopAndDestroy( contentUri );

    if( rights == NULL )
        {
        if( err == DRMCommon::ENoRights )
            {
            // Handle "No rights" situation here
            iDrmHelper->HandleErrorL( DRMCommon::ENoRights, aFileName );
            return result;
            }
        else
            {
            User::Leave( KErrGeneral );
            }
        }

    CleanupStack::PushL( rights );

    switch ( err )
		{
		case CDRMRights::EFullRights: // Fall through
		case CDRMRights::ERestrictedRights:
			{
			CDRMRights::TRestriction restriction; // ignore
			CDRMRights::TExpiration expiration;
			TUint32 constType;
			
			rights->GetRightsInfo( DRMCommon::EPlay, restriction, // ignore return
								expiration, constType );       // value
		
			// This is CFM protected file, ringingtone is set to "no"
			if ( ( rights->GetPermission().iInfoBits & ENoRingingTone )
				// This is normal DRM protected count-based tone
				|| ( constType & CDRMRights::ECountBased ) )
				{
				ShowErrorNoteL( R_PROFILE_TEXT_DRM_PREV_RIGHTS_SET );
				}		
			else
				{
				switch ( expiration )
					{
					case CDRMRights::EValidRights:
						{
						result = KErrNone;
						break;
						}

					case CDRMRights::EFutureRights:
						{
						iDrmHelper->HandleErrorL( DRMCommon::ENoRights, aFileName );
						break;
						}

					case CDRMRights::EExpiredRights: 
						{
						iDrmHelper->HandleErrorL( DRMCommon::ERightsExpired, aFileName );
						break;
						}

					default:
						{
						break;
						}
					}	

				}
				

			break;
			}

		case CDRMRights::EPreviewRights:
			{
			ShowErrorNoteL( R_PROFILE_TEXT_DRM_PREV_RIGHTS_SET );
			break;
			}

		case DRMCommon::ENoRights:
			{
			iDrmHelper->HandleErrorL( DRMCommon::ENoRights, aFileName );
			break;
			}

		default:
			{
			break;
			}

		}
   
    CleanupStack::PopAndDestroy( rights );

    return result;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CanSetAutomated
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::CanSetAutomated( const TDesC& aFileName )
    {
    TBool canSetAutomated( EFalse );
    return
        iDrmHelper->CanSetAutomated( aFileName, canSetAutomated ) == KErrNone &&
        canSetAutomated;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::AskAutomated
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::AskAutomated( const TDesC& aFileName )
    {
    return iDrmHelper->ShowAutomatedNote( aFileName );
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetAutomated
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::SetAutomated( TProfileTones aToneSetting,
                                        const TDesC& aFileName )
    {
    TInt err = KErrNone;
    
    TRAP_IGNORE( err = SetAutomatedL( aToneSetting, aFileName ) );

    return err;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetAutomatedL
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::SetAutomatedL( TProfileTones aToneSetting,
                                        const TDesC& aFileName )
    {
    iDrmHelper->SetAutomatedType( ToneSettingToAutomatedType( aToneSetting ) );
    TInt err( iDrmHelper->SetAutomatedSilent( aFileName, EFalse ) );
    if( err == KErrNone )
        {
        using namespace ContentAccess;
       	TVirtualPathPtr path( aFileName, KDefaultContentObject );
        CData* data = CData::NewLC( path, EContentShareReadWrite );
        err = data->ExecuteIntent( EPlay );
        CleanupStack::PopAndDestroy( data );
        // According to Risto Vilkman from DRM, KErrAccessDenied
        // can be ignored at this point, since if
        // CanSetAutomated says the tone is OK, it's OK.
        if( err == KErrAccessDenied )
        	{
        	err = KErrNone;
        	}
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::RemoveAutomated
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::RemoveAutomated( const TDesC& aFileName )
    {
    iDrmHelper->RemoveAutomatedPassive( aFileName ); // ignore return value
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::IsProtected
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::IsProtected( const TDesC& aFileName ) const
    {
    TBool result = EFalse;
    TRAP_IGNORE( result = IsProtectedL( aFileName ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::IsProtectedL
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::IsProtectedL( const TDesC& aFileName ) const
    {
    TInt isProtected( EFalse );
   	ContentAccess::TVirtualPathPtr path( aFileName,
   						ContentAccess::KDefaultContentObject );
   	CData* data = CData::NewLC( path, EContentShareReadWrite );
   	TInt result = data->GetAttribute( EIsProtected, isProtected );
   	CleanupStack::PopAndDestroy(); // data

    return( ( result == DRMCommon::EOk ) && isProtected );
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
LOCAL_C void FormatGUID( TDes8 &aGUID )
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
    // Check on runtime wether WM DRM is supporeted or not

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
//
// CProfileToneHandler::IsFileWMDRMProtectedL
//
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::IsFileWMDRMProtectedL( const TDesC& aFileName )
	{
	TBool res = EFalse;
	RFile hFile;

	TInt err = hFile.Open( iFs, aFileName, 
						EFileRead | EFileStream | EFileShareReadersOnly );
	if( err == KErrInUse )
		{
		err = hFile.Open( iFs, aFileName, 
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

//  End of File
