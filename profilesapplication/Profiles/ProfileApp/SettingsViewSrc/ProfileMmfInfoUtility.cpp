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
* Description:   A wrapper interface around MMF (Multimedia Framework) to get
*                information about MIME-types supported by MMF.
*
*/



// CLASS HEADER
#include    "ProfileMmfInfoUtility.h"

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32std.h>
#include <badesca.h>
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <caf/caf.h>
namespace
	{
	// CONSTANTS
	_LIT8( KProfileRngMimeType,    "application/vnd.nokia.ringing-tone" );
	//_LIT8( KProfilePlainTextType,  "text/plain" );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileMmfInfoUtility::IsMimeTypeSupported
// -----------------------------------------------------------------------------
//
TBool ProfileMmfInfoUtility::IsMimeTypeSupportedL( const TDesC8& aMimeType )
    {

    //if( aMimeType.CompareF( KProfilePlainTextType ) == 0 )
    //    {
    //    return EFalse;
    //    }

    if( aMimeType.CompareF( KProfileRngMimeType ) == 0 )
        {
        return ETrue;
        }

    TBool result( EFalse );

	CMMFFormatSelectionParameters* formatPrms =
        CMMFFormatSelectionParameters::NewLC();
	CMMFControllerPluginSelectionParameters* controllerPrms =
        CMMFControllerPluginSelectionParameters::NewLC();

    // Empty format parameters means: "get all the supported formats"
    controllerPrms->SetRequiredPlayFormatSupportL( *formatPrms );
    RMMFControllerImplInfoArray cntrlArray;
    controllerPrms->ListImplementationsL( cntrlArray );

    for( TInt i( cntrlArray.Count() - 1 ); i >= 0 && !result; --i )
        {
        const RMMFFormatImplInfoArray&
            infoArray( cntrlArray[i]->PlayFormats() );

        for( TInt j( infoArray.Count() - 1 ); j >= 0; --j )
            {
            if( infoArray[j]->SupportsMimeType( aMimeType ) )
                {
                result = ETrue;
                break;
                }
            }
        }
	
	cntrlArray.ResetAndDestroy();
	cntrlArray.Close();
    CleanupStack::PopAndDestroy( 2, formatPrms );

    return result;
    }

// -----------------------------------------------------------------------------
// CProfileMmfInfoUtility::IsHeaderDataSupportedL
// -----------------------------------------------------------------------------
//
TBool ProfileMmfInfoUtility::IsHeaderDataSupportedL( const TDesC& aFileName )
	{
	TBool result( EFalse );
	//the max header length is 256 bytes
	const TInt KMaxHeaderLength( 256 );
	HBufC8* header = HBufC8::NewLC( KMaxHeaderLength );
	TPtr8 headerPtr = header->Des();
	GetFileHeaderDataL( aFileName, headerPtr, KMaxHeaderLength );
	CMMFFormatSelectionParameters* formatPrms =
			CMMFFormatSelectionParameters::NewLC();
	CMMFControllerPluginSelectionParameters* controllerPrms =
			CMMFControllerPluginSelectionParameters::NewLC();

	// Empty format parameters means: "get all the supported formats"
	controllerPrms->SetRequiredPlayFormatSupportL( *formatPrms );
	RMMFControllerImplInfoArray cntrlArray;
	controllerPrms->ListImplementationsL( cntrlArray );

	for ( TInt i( cntrlArray.Count() - 1 ); i >= 0 && !result; --i )
		{
		const RMMFFormatImplInfoArray& infoArray( cntrlArray[i]->PlayFormats() );

		for ( TInt j(infoArray.Count() - 1); j >= 0; --j )
			{
			if ( infoArray[j]->SupportsHeaderDataL( *header ) )
				{
				result = ETrue;
				break;
				}
			}
		}

	cntrlArray.ResetAndDestroy();
	cntrlArray.Close();
	CleanupStack::PopAndDestroy( 3, header );//controllerPrms, formatPrms and header

	return result;
	}

// -----------------------------------------------------------------------------
// CProfileMmfInfoUtility::GetFileHeaderDataL
// -----------------------------------------------------------------------------
//
void ProfileMmfInfoUtility::GetFileHeaderDataL( const TDesC& aFileName,
		TDes8& aHeaderData, TInt aMaxHeaderLength )
	{
	TInt error = KErrNone;
	using namespace ContentAccess;
	TVirtualPathPtr path( aFileName, ContentAccess::KDefaultContentObject );
	CData* data = CData::NewLC( path, EContentShareReadWrite );
	TInt size = 0;
	data->DataSizeL( size );
	if ( size > 0 )
		{
		if ( size > aMaxHeaderLength )
			size = aMaxHeaderLength;
		TInt pos = 0;
		error = data->Seek( ESeekStart, pos );
		error = data->Read( aHeaderData, size );
		}
	CleanupStack::PopAndDestroy(); // data
	}

//  End of File

