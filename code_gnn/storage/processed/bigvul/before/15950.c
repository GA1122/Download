static void ExportTIFF_PhotographicSensitivity ( SXMPMeta * xmp, TIFF_Manager * exif ) {

	
	try {
	
		bool foundXMP, foundExif;
		TIFF_Manager::TagInfo tagInfo;
		std::string xmpValue;
		XMP_OptionBits flags;
		XMP_Int32 binValue = 0;
	
		bool haveOldExif = true;	 
	
		foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo );
		if ( foundExif && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
			haveOldExif = (strncmp ( (char*)tagInfo.dataPtr, "0230", 4 ) < 0);
		}
	
		if ( haveOldExif ) {	 
		
			foundXMP = xmp->GetProperty ( kXMP_NS_EXIF, "ISOSpeedRatings", 0, &flags );
			if ( foundXMP && XMP_PropIsArray(flags) && (xmp->CountArrayItems ( kXMP_NS_EXIF, "ISOSpeedRatings" ) > 0) ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_EXIF, "ISOSpeedRatings[1]", &binValue, 0 );
			}
			
			if ( ! foundXMP ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "PhotographicSensitivity", &binValue, 0 );
			}

			if ( foundXMP && (0 <= binValue) && (binValue <= 65535) ) {
				xmp->DeleteProperty ( kXMP_NS_EXIF, "ISOSpeedRatings" );	 
				foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, &tagInfo );
				if ( ! foundExif ) {
					exif->SetTag_Short ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, (XMP_Uns16)binValue );
				}
			}
		
		} else {	 
		

			if ( ! xmp->DoesPropertyExist ( kXMP_NS_ExifEX,  "PhotographicSensitivity" ) ) {
				foundXMP = xmp->GetProperty ( kXMP_NS_EXIF, "ISOSpeedRatings", 0, &flags );
				if ( foundXMP && XMP_PropIsArray(flags) &&
					 (xmp->CountArrayItems ( kXMP_NS_EXIF, "ISOSpeedRatings" ) > 0) ) {
					xmp->GetArrayItem ( kXMP_NS_EXIF, "ISOSpeedRatings", 1, &xmpValue, 0 );
					xmp->SetProperty ( kXMP_NS_ExifEX,  "PhotographicSensitivity", xmpValue.c_str() );
				}
			}
			
			xmp->DeleteProperty ( kXMP_NS_EXIF, "ISOSpeedRatings" );	 
			
			foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "PhotographicSensitivity", &binValue, 0 );

			if ( foundXMP && (0 <= binValue) && (binValue <= 65535) ) {	 
			
				foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, &tagInfo );
				if ( ! foundExif ) {
					exif->SetTag_Short ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, (XMP_Uns16)binValue );
				}
			
			} else if ( foundXMP ) {	 
			
				bool havePhotographicSensitivityTag = exif->GetTag ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, 0 );
				bool haveSensitivityTypeTag = exif->GetTag ( kTIFF_ExifIFD, kTIFF_SensitivityType, 0 );
				bool haveISOSpeedTag = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ISOSpeed, 0 );
			
				bool haveSensitivityTypeXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "SensitivityType" );
				bool haveISOSpeedXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "ISOSpeed" );
			
				if ( (! havePhotographicSensitivityTag) && (! haveSensitivityTypeTag) && (! haveISOSpeedTag) ) {

					exif->SetTag_Short ( kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, 65535 );
					
					if ( (! haveSensitivityTypeXMP) && (! haveISOSpeedXMP) ) {
						xmp->SetProperty ( kXMP_NS_ExifEX, "SensitivityType", "3" );
						xmp->SetProperty_Int ( kXMP_NS_ExifEX, "ISOSpeed", binValue );
					}

				}
			
			}
	
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_SensitivityType, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "SensitivityType", &binValue, 0 );
				if ( foundXMP && (0 <= binValue) && (binValue <= 65535) ) {
					exif->SetTag_Short ( kTIFF_ExifIFD, kTIFF_SensitivityType, (XMP_Uns16)binValue );
				}
			}
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_StandardOutputSensitivity, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "StandardOutputSensitivity", &binValue, 0 );
				if ( foundXMP && (binValue >= 0) ) {
					exif->SetTag_Long ( kTIFF_ExifIFD, kTIFF_StandardOutputSensitivity, (XMP_Uns32)binValue );
				}
			}
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_RecommendedExposureIndex, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "RecommendedExposureIndex", &binValue, 0 );
				if ( foundXMP && (binValue >= 0) ) {
					exif->SetTag_Long ( kTIFF_ExifIFD, kTIFF_RecommendedExposureIndex, (XMP_Uns32)binValue );
				}
			}
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ISOSpeed, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "ISOSpeed", &binValue, 0 );
				if ( foundXMP && (binValue >= 0) ) {
					exif->SetTag_Long ( kTIFF_ExifIFD, kTIFF_ISOSpeed, (XMP_Uns32)binValue );
				}
			}
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudeyyy, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "ISOSpeedLatitudeyyy", &binValue, 0 );
				if ( foundXMP && (binValue >= 0) ) {
					exif->SetTag_Long ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudeyyy, (XMP_Uns32)binValue );
				}
			}
	
			foundExif = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudezzz, &tagInfo );
			if ( ! foundExif ) {
				foundXMP = xmp->GetProperty_Int ( kXMP_NS_ExifEX, "ISOSpeedLatitudezzz", &binValue, 0 );
				if ( foundXMP && (binValue >= 0) ) {
					exif->SetTag_Long ( kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudezzz, (XMP_Uns32)binValue );
				}
			}
		
		}

	} catch ( ... ) {


	}

}	 
