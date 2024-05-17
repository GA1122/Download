static void ImportTIFF_PhotographicSensitivity ( const TIFF_Manager & exif, SXMPMeta * xmp ) {

	
	try {
	
		bool found;
		TIFF_Manager::TagInfo tagInfo;
	
		bool haveOldExif = true;	 
		bool haveTag34855 = false;
		bool haveLowISO = false;	 
		
		XMP_Uns32 valueTag34855;	 
	
		found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo );
		if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
			haveOldExif = (strncmp ( (char*)tagInfo.dataPtr, "0230", 4 ) < 0);
		}
	
		haveTag34855 = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, &valueTag34855 );
		if ( haveTag34855 ) haveLowISO = (valueTag34855 < 65535);

		if ( haveOldExif ) {	 

			if ( haveTag34855 ) {
				if ( haveLowISO || (! xmp->DoesPropertyExist ( kXMP_NS_EXIF, "ISOSpeedRatings" )) ) {
					xmp->DeleteProperty ( kXMP_NS_EXIF, "ISOSpeedRatings" );
					xmp->AppendArrayItem ( kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "" );
					xmp->SetProperty_Int ( kXMP_NS_EXIF, "ISOSpeedRatings[1]", valueTag34855 );
				}
			}
		
		} else {	 
		
	
			XMP_Uns16 whichLongTag = 0;
			XMP_Uns32 sensitivityType, tagValue;
			
			bool haveSensitivityType = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_SensitivityType, &sensitivityType );
			if ( haveSensitivityType ) {
				xmp->SetProperty_Int ( kXMP_NS_ExifEX, "SensitivityType", sensitivityType );
				switch ( sensitivityType ) {
					case 1  :  
					case 4  : whichLongTag = kTIFF_StandardOutputSensitivity; break;
					case 2  : whichLongTag = kTIFF_RecommendedExposureIndex; break;
					case 3  :  
					case 5  :
					case 6  :
					case 7  : whichLongTag = kTIFF_ISOSpeed; break;
				}
			}
			
			found = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_StandardOutputSensitivity, &tagValue );
			if ( found ) {
				xmp->SetProperty_Int64 ( kXMP_NS_ExifEX, "StandardOutputSensitivity", tagValue );
			}
			
			found = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_RecommendedExposureIndex, &tagValue );
			if ( found ) {
				xmp->SetProperty_Int64 ( kXMP_NS_ExifEX, "RecommendedExposureIndex", tagValue );
			}
			
			found = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_ISOSpeed, &tagValue );
			if ( found ) {
				xmp->SetProperty_Int64 ( kXMP_NS_ExifEX, "ISOSpeed", tagValue );
			}
			
			found = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudeyyy, &tagValue );
			if ( found ) {
				xmp->SetProperty_Int64 ( kXMP_NS_ExifEX, "ISOSpeedLatitudeyyy", tagValue );
			}
			
			found = exif.GetTag_Integer ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudezzz, &tagValue );
			if ( found ) {
				xmp->SetProperty_Int64 ( kXMP_NS_ExifEX, "ISOSpeedLatitudezzz", tagValue );
			}
		
	
			if ( haveTag34855 & haveLowISO ) {	 
	
				xmp->DeleteProperty ( kXMP_NS_EXIF, "ISOSpeedRatings" );
				xmp->AppendArrayItem ( kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "" );
				xmp->SetProperty_Int ( kXMP_NS_EXIF, "ISOSpeedRatings[1]", valueTag34855 );
				xmp->SetProperty_Int ( kXMP_NS_ExifEX, "PhotographicSensitivity", valueTag34855 );
	
			} else {	 
				
				if ( haveTag34855 ) {
					XMP_Assert ( valueTag34855 == 65535 );
					xmp->SetProperty_Int ( kXMP_NS_ExifEX, "PhotographicSensitivity", valueTag34855 );
				}
				
				if ( whichLongTag != 0 ) {
					found = exif.GetTag ( kTIFF_ExifIFD, whichLongTag, &tagInfo );
					if ( found && (tagInfo.type == kTIFF_LongType) && (tagInfo.count == 1) ){
						xmp->DeleteProperty ( kXMP_NS_EXIF, "ISOSpeedRatings" );
						xmp->AppendArrayItem ( kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "" );
						xmp->SetProperty_Int ( kXMP_NS_EXIF, "ISOSpeedRatings[1]", exif.GetUns32 ( tagInfo.dataPtr ) );
					}
				}
	
			}
		
		}
	
	} catch ( ... ) {
	
		
	}

}	 
