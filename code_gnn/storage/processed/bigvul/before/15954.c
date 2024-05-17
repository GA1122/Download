PhotoDataUtils::Import2WayExif ( const TIFF_Manager & exif, SXMPMeta * xmp, int iptcDigestState )
{
	const bool nativeEndian = exif.IsNativeEndian();

	bool found, foundFromXMP;
	TIFF_Manager::TagInfo tagInfo;
	XMP_OptionBits flags;

	ImportTIFF_StandardMappings ( kTIFF_PrimaryIFD, exif, xmp );
	ImportTIFF_StandardMappings ( kTIFF_ExifIFD, exif, xmp );
	ImportTIFF_StandardMappings ( kTIFF_GPSInfoIFD, exif, xmp );
	
	
	#if SupportOldExifProperties
		
		found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_CameraOwnerName, &tagInfo );
		if ( found && (tagInfo.type == kTIFF_ASCIIType) && (tagInfo.count > 0) ) {
			ImportSingleTIFF ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF_Aux, "OwnerName" );
		}
		
		found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_BodySerialNumber, &tagInfo );
		if ( found && (tagInfo.type == kTIFF_ASCIIType) && (tagInfo.count > 0) ) {
			ImportSingleTIFF ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF_Aux, "SerialNumber" );
		}
		
		found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_LensModel, &tagInfo );
		if ( found && (tagInfo.type == kTIFF_ASCIIType) && (tagInfo.count > 0) ) {
			ImportSingleTIFF ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF_Aux, "Lens" );
		}
		
		found = xmp->GetProperty ( kXMP_NS_ExifEX, "LensSpecification", 0, &flags );
		if ( found && XMP_PropIsArray(flags) ) {
			std::string fullStr, oneItem;
			size_t count = (size_t) xmp->CountArrayItems ( kXMP_NS_ExifEX, "LensSpecification" );
			if ( count > 0 ) {
				(void) xmp->GetArrayItem ( kXMP_NS_ExifEX, "LensSpecification", 1, &fullStr, 0 );
				for ( size_t i = 2; i <= count; ++i ) {
					fullStr += ' ';
					(void) xmp->GetArrayItem ( kXMP_NS_ExifEX, "LensSpecification", i, &oneItem, 0 );
					fullStr += oneItem;
				}
			}
			xmp->SetProperty ( kXMP_NS_EXIF_Aux, "LensInfo", fullStr.c_str(), kXMP_DeleteExisting );
		}
	
	#endif
	

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSAltitude, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_RationalType) &&  (tagInfo.count == 1) ) {

		XMP_Uns32 num = exif.GetUns32 ( tagInfo.dataPtr );
		XMP_Uns32 denom = exif.GetUns32 ( (XMP_Uns8*)tagInfo.dataPtr + 4 );
		
		bool fixXMP = false;
		bool numNeg = num >> 31;
		bool denomNeg = denom >> 31;
		
		if ( denomNeg ) {	 
			denom = -denom;
			num = -num;
			numNeg = num >> 31;
			fixXMP = true;
		}

		if ( numNeg ) {	 
			xmp->SetProperty ( kXMP_NS_EXIF, "GPSAltitudeRef", "1" );
			num = -num;
			fixXMP = true;
		}

		if ( fixXMP ) {
			char buffer [32];
			snprintf ( buffer, sizeof(buffer), "%lu/%lu", (unsigned long) num, (unsigned long) denom );	 
			xmp->SetProperty ( kXMP_NS_EXIF, "GPSAltitude", buffer );
		}

	}
	

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_DateTimeOriginal, &tagInfo );
	foundFromXMP = xmp->DoesPropertyExist ( kXMP_NS_EXIF, "DateTimeOriginal" );
	
	if ( found && (! foundFromXMP) && (tagInfo.type == kTIFF_ASCIIType) ) {
		ImportTIFF_Date ( exif, tagInfo, xmp, kXMP_NS_EXIF, "DateTimeOriginal" );
	}

	found = exif.GetTag ( kTIFF_PrimaryIFD, kTIFF_DateTime, &tagInfo );
	foundFromXMP = xmp->DoesPropertyExist ( kXMP_NS_XMP, "ModifyDate" );
	
	if ( found && (! foundFromXMP) && (tagInfo.type == kTIFF_ASCIIType) ) {
		ImportTIFF_Date ( exif, tagInfo, xmp, kXMP_NS_XMP, "ModifyDate" );
	}


	ImportTIFF_PhotographicSensitivity ( exif, xmp );
	
	found = exif.GetTag ( kTIFF_PrimaryIFD, kTIFF_Artist, &tagInfo );
	foundFromXMP = xmp->DoesPropertyExist ( kXMP_NS_DC, "creator" );
	if ( (! found) && (! foundFromXMP) ) {
		found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_CameraOwnerName, &tagInfo );
		if ( found ) {
			std::string xmpValue ( (char*)tagInfo.dataPtr, tagInfo.dataLen );
			xmp->AppendArrayItem ( kXMP_NS_DC, "creator", kXMP_PropArrayIsOrdered, xmpValue.c_str() );
		}
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
		char str[5];

		*((XMP_Uns32*)str) = GetUns32AsIs ( tagInfo.dataPtr );
		str[4] = 0;
		xmp->SetProperty ( kXMP_NS_EXIF, "ExifVersion", str );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_FlashpixVersion, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
		char str[5];

		*((XMP_Uns32*)str) = GetUns32AsIs ( tagInfo.dataPtr );
		str[4] = 0;
		xmp->SetProperty ( kXMP_NS_EXIF, "FlashpixVersion", str );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_ComponentsConfiguration, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
		ImportArrayTIFF_Byte ( tagInfo, xmp, kXMP_NS_EXIF, "ComponentsConfiguration" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_UserComment, &tagInfo );
	if ( found ) {
		ImportTIFF_EncodedString ( exif, tagInfo, xmp, kXMP_NS_EXIF, "UserComment", true   );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_OECF, &tagInfo );
	if ( found ) {
		ImportConversionTable ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF, "OECF" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_Flash, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_ShortType) && (tagInfo.count == 1) ) {
		ImportTIFF_Flash ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF, "Flash" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_SpatialFrequencyResponse, &tagInfo );
	if ( found ) {
		ImportConversionTable ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF, "SpatialFrequencyResponse" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_FileSource, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 1) ) {
		ImportSingleTIFF_Byte ( tagInfo, xmp, kXMP_NS_EXIF, "FileSource" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_SceneType, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 1) ) {
		ImportSingleTIFF_Byte ( tagInfo, xmp, kXMP_NS_EXIF, "SceneType" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_CFAPattern, &tagInfo );
	if ( found ) {
		ImportTIFF_CFATable ( tagInfo, nativeEndian, xmp, kXMP_NS_EXIF, "CFAPattern" );
	}

	found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_DeviceSettingDescription, &tagInfo );
	if ( found ) {
		ImportTIFF_DSDTable ( exif, tagInfo, xmp, kXMP_NS_EXIF, "DeviceSettingDescription" );
	}


	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSVersionID, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_ByteType) && (tagInfo.count == 4) ) {
		const XMP_Uns8 * binValue = (const XMP_Uns8 *) tagInfo.dataPtr;
		char strOut[20]; 
		snprintf ( strOut, sizeof(strOut), "%u.%u.%u.%u",	 
				   binValue[0], binValue[1], binValue[2], binValue[3] );
		xmp->SetProperty ( kXMP_NS_EXIF, "GPSVersionID", strOut );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSLatitude, &tagInfo );
	if ( found ) {
		ImportTIFF_GPSCoordinate ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSLatitude" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSLongitude, &tagInfo );
	if ( found ) {
		ImportTIFF_GPSCoordinate ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSLongitude" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSTimeStamp, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_RationalType) && (tagInfo.count == 3) ) {
		ImportTIFF_GPSTimeStamp ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSTimeStamp" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDestLatitude, &tagInfo );
	if ( found ) {
		ImportTIFF_GPSCoordinate ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSDestLatitude" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDestLongitude, &tagInfo );
	if ( found ) {
		ImportTIFF_GPSCoordinate ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSDestLongitude" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSProcessingMethod, &tagInfo );
	if ( found ) {
		ImportTIFF_EncodedString ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSProcessingMethod" );
	}

	found = exif.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSAreaInformation, &tagInfo );
	if ( found ) {
		ImportTIFF_EncodedString ( exif, tagInfo, xmp, kXMP_NS_EXIF, "GPSAreaInformation" );
	}

}	 
