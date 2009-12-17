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
* Description:  Data structure class for profile settings.
*
*/



// CLASS HEADER
#include "CProfileStruct.h"
#include "profileinternal.hrh"

// EXTERNAL INCLUDES
#include <mprofileextended.h>
#include <mprofilename.h>
#include <mprofilesetname.h>
#include <mprofiletones.h>
#include <MProfileSetTones.h>
#include <mprofileextratones.h>
#include <mprofilesetextratones.h>
#include <mprofileextrasettings.h>
#include <mprofilesetextrasettings.h>
#include <mprofilepttsettings.h>
#include <mprofilesetpttsettings.h>
#include <AknUtils.h> // AknTextUtils
#include <mprofilefeedbacksettings.h>
#include <mprofilesetfeedbacksettings.h>
#include <mprofile3dtonesettings.h>
#include <mprofileset3dtonesettings.h>

#include <tprofiletonesettings.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileStruct::CProfileStruct
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileStruct::CProfileStruct(  )
    {
    }
    
// -----------------------------------------------------------------------------
// CProfileStruct::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileStruct* CProfileStruct::NewL( )
    {
    CProfileStruct* self = new (ELeave) CProfileStruct( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CProfileStruct::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CProfileStruct::ConstructL()
	{
	iRingingTone = new( ELeave ) TFileName();
	iRingingToneLine2 = new( ELeave ) TFileName();
	iVideoCallTone = new( ELeave ) TFileName();
	iMessageAlertTone = new( ELeave ) TFileName();
	iEmailAlertTone = new( ELeave ) TFileName();
	iPTTRingingTone = new( ELeave ) TFileName();
	}
	    
// Destructor
CProfileStruct::~CProfileStruct()
    {
    iContactList.Close();
    delete iRingingTone;
    delete iRingingToneLine2;
	delete iVideoCallTone;
	delete iMessageAlertTone;
    delete iEmailAlertTone;
	delete iPTTRingingTone;
    }

// -----------------------------------------------------------------------------
// CProfileStruct::LoadL
// -----------------------------------------------------------------------------
//
void CProfileStruct::LoadL( MProfileExtended* aProfile )
    {
    TArray<TContactItemId> array( aProfile->AlertForL() );
    iContactList.Reset();
    TInt count( array.Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        iContactList.Append( array[ index ] );
        }
    const MProfileName& name = aProfile->ProfileName();
    iProfileId = name.Id();
    iProfileName.Copy( name.Name() );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( iProfileName );
    iTitleName.Copy( name.TitleName() );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( iTitleName );
    const MProfileTones& tones = aProfile->ProfileTones();
    iRingingTone->Copy( tones.RingingTone1() );
    iRingingToneLine2->Copy( tones.RingingTone2() );
    iMessageAlertTone->Copy( tones.MessageAlertTone() );
    const TProfileToneSettings& toneSettings = tones.ToneSettings();
    iRingingType = toneSettings.iRingingType;
    iKeypadVolume = toneSettings.iKeypadVolume;
    iRingingVolume = toneSettings.iRingingVolume;
	iMediaVolume = toneSettings.iMediaVolume;
    iVibratingAlert = toneSettings.iVibratingAlert;
 iEmailVibratingAlert = toneSettings.iEmailVibratingAlert;
    iWarningAndGameTones = toneSettings.iWarningAndGameTones;
	iTextToSpeech = toneSettings.iTextToSpeech;
    const MProfileExtraTones& extra = aProfile->ProfileExtraTones();
    iEmailAlertTone->Copy( extra.EmailAlertTone() );
    iVideoCallTone->Copy( extra.VideoCallRingingTone() );
    const MProfileExtraSettings& extraSettings = aProfile->ProfileExtraSettings();
    const MProfilePttSettings& pttSettings = extraSettings.ProfilePttSettings();
    iPTTRingingTone->Copy( pttSettings.PttRingingTone() );
	iPTTStatus = pttSettings.PttStatus();

    const MProfileFeedbackSettings& feedbackSettings = extraSettings.ProfileFeedbackSettings();
    iTactileFeedback = feedbackSettings.TactileFeedback();
    iAudioFeedback = feedbackSettings.AudioFeedback();
	
    const MProfile3DToneSettings& threeDSettings = extraSettings.Profile3DToneSettings();
    i3DEffect = threeDSettings.Effect();
    i3DEcho = threeDSettings.Echo();
    }

// -----------------------------------------------------------------------------
// CProfileStruct::StoreL
// -----------------------------------------------------------------------------
//
void CProfileStruct::StoreL( MProfileExtended* aProfile )
    {
    aProfile->SetAlertForL( iContactList.Array() );
    const MProfileName& name = aProfile->ProfileName();
    TBuf<KProfileMaxProfileNameLength> profileNameHelperBuffer;
    profileNameHelperBuffer.Copy( name.Name() );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion(
            profileNameHelperBuffer );
    if( iProfileName.Compare( profileNameHelperBuffer ) )
        {
        // Store profile name only if it has changed in order to
        // preserve localisation.
        aProfile->ProfileSetName().SetNameL( iProfileName );
        iTitleName.Copy( iProfileName );
        }
    MProfileSetTones& setTones = aProfile->ProfileSetTones();
    setTones.SetRingingTone1L( *iRingingTone );
    setTones.SetRingingTone2L( *iRingingToneLine2 );
    setTones.SetMessageAlertToneL( *iMessageAlertTone );
    TProfileToneSettings& toneSettings = setTones.SetToneSettings();
    toneSettings.iRingingType = TProfileRingingType( iRingingType );
    toneSettings.iKeypadVolume = TProfileKeypadVolume( iKeypadVolume );
    toneSettings.iRingingVolume = iRingingVolume;
	toneSettings.iMediaVolume = iMediaVolume;
    toneSettings.iVibratingAlert = iVibratingAlert;
    toneSettings.iEmailVibratingAlert = iEmailVibratingAlert;
    toneSettings.iWarningAndGameTones = iWarningAndGameTones;
	toneSettings.iTextToSpeech = iTextToSpeech;
    MProfileSetExtraTones& setExtra = aProfile->ProfileSetExtraTones();
    setExtra.SetEmailAlertToneL( *iEmailAlertTone );
    setExtra.SetVideoCallRingingToneL( *iVideoCallTone );

    MProfileSetExtraSettings& setExtraSettings = aProfile->ProfileSetExtraSettings();
    MProfileSetPttSettings& setPttSettings = setExtraSettings.ProfileSetPttSettings();
	setPttSettings.SetPttRingingToneL( *iPTTRingingTone );
	TProfilePttStatus KPttStatus = TProfilePttStatus( iPTTStatus );
	setPttSettings.SetPttStatus( KPttStatus );
    
    MProfileSetFeedbackSettings& setFeedbackSettings =
        setExtraSettings.ProfileSetFeedbackSettings();
    TProfileTactileFeedback tactileFeedback = TProfileTactileFeedback( iTactileFeedback );
    TProfileAudioFeedback audioFeedback = TProfileAudioFeedback( iAudioFeedback );
    setFeedbackSettings.SetTactileFeedback( tactileFeedback );
    setFeedbackSettings.SetAudioFeedback( audioFeedback );
    
    MProfileSet3DToneSettings& set3dSettings =
        setExtraSettings.ProfileSet3DToneSettings();
    TProfile3DToneEffect threeDEffect = TProfile3DToneEffect( i3DEffect );
    TProfile3DToneEcho threeDEcho = TProfile3DToneEcho( i3DEcho );
    set3dSettings.SetEffect( threeDEffect );
    set3dSettings.SetEcho( threeDEcho );
    }

// -----------------------------------------------------------------------------
// CProfileStruct::CompareL
// -----------------------------------------------------------------------------
//
TBool CProfileStruct::CompareL( MProfileExtended* aProfile ) const
	{
    const MProfileTones& tones = aProfile->ProfileTones();
    const MProfileName& name = aProfile->ProfileName();
    const TProfileToneSettings& toneSettings = tones.ToneSettings();
    const MProfileExtraTones& extra = aProfile->ProfileExtraTones();
	const MProfileExtraSettings& extraSettings = aProfile->ProfileExtraSettings();
    const MProfilePttSettings& pttSettings = extraSettings.ProfilePttSettings();
    const MProfileFeedbackSettings& feedbackSettings = extraSettings.ProfileFeedbackSettings();    
    const MProfile3DToneSettings& threeDSettings = extraSettings.Profile3DToneSettings();
	
    TArray<TContactItemId> array( aProfile->AlertForL() );
    TInt contactArrayCount( array.Count() );
	TInt ownContactArrayCount( iContactList.Count() );

    if( ( iProfileId != name.Id() ) ||
		( iRingingTone->Compare( tones.RingingTone1() ) ) ||
	    ( iRingingToneLine2->Compare( tones.RingingTone2() ) ) ||
        ( iVideoCallTone->Compare( extra.VideoCallRingingTone() ) ) ||
		( iMessageAlertTone->Compare( tones.MessageAlertTone() ) ) ||
		( iProfileName.Compare( name.Name() ) ) ||
		( iRingingType != toneSettings.iRingingType ) ||
		( iKeypadVolume != toneSettings.iKeypadVolume ) ||
		( iRingingVolume != toneSettings.iRingingVolume ) ||
		( iMediaVolume != toneSettings.iMediaVolume ) ||
		( iVibratingAlert != toneSettings.iVibratingAlert ) ||
		( iEmailVibratingAlert != toneSettings.iEmailVibratingAlert ) ||
		( iWarningAndGameTones != toneSettings.iWarningAndGameTones ) ||
		( iTextToSpeech != toneSettings.iTextToSpeech ) ||
        ( iEmailAlertTone->Compare( extra.EmailAlertTone() ) ) ||
		( ownContactArrayCount != contactArrayCount ) ||
		( iTactileFeedback != feedbackSettings.TactileFeedback() ) ||
		( iAudioFeedback != feedbackSettings.AudioFeedback() ) ||
    	( i3DEffect != threeDSettings.Effect() ) ||
		( i3DEcho != threeDSettings.Echo() ) ||
		( iPTTRingingTone->Compare( pttSettings.PttRingingTone() ) ) ||
		( iPTTStatus != pttSettings.PttStatus() ) )
		{
		return EFalse;
		}

    for( TInt index( 0 ); index < contactArrayCount; index++ )
        {
        if( iContactList[ index ] != array[ index ] )
			{
			return EFalse;
			}
        }
	return ETrue;
	}

// End of File
