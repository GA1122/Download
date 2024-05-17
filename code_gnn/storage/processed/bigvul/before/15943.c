PhotoDataUtils::ExportExif ( SXMPMeta * xmp, TIFF_Manager * exif )
{
	bool haveXMP;
	std::string xmpValue;

	XMP_Int32 int32;
	XMP_Uns8 uns8;
	
	
	#if SupportOldExifProperties
	
		XMP_OptionBits flags;
	
		haveXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "PhotographicSensitivity" );
		if ( ! haveXMP ) {
			haveXMP = xmp->GetProperty ( kXMP_NS_EXIF, "ISOSpeedRatings", 0, &flags );
			if ( haveXMP && XMP_PropIsArray(flags) ) {
				haveXMP = xmp->GetArrayItem ( kXMP_NS_EXIF, "ISOSpeedRatings", 1, &xmpValue, 0 );
				if ( haveXMP ) xmp->SetProperty ( kXMP_NS_ExifEX, "PhotographicSensitivity", xmpValue.c_str() );
			}
		}
	
		haveXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "CameraOwnerName" );
		if ( ! haveXMP ) {
			haveXMP = xmp->GetProperty ( kXMP_NS_EXIF_Aux, "OwnerName", &xmpValue, 0 );
			if ( haveXMP ) xmp->SetProperty ( kXMP_NS_ExifEX, "CameraOwnerName", xmpValue.c_str() );
		}
	
		haveXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "BodySerialNumber" );
		if ( ! haveXMP ) {
			haveXMP = xmp->GetProperty ( kXMP_NS_EXIF_Aux, "SerialNumber", &xmpValue, 0 );
			if ( haveXMP ) xmp->SetProperty ( kXMP_NS_ExifEX, "BodySerialNumber", xmpValue.c_str() );
		}
	
		haveXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "LensModel" );
		if ( ! haveXMP ) {
			haveXMP = xmp->GetProperty ( kXMP_NS_EXIF_Aux, "Lens", &xmpValue, 0 );
			if ( haveXMP ) xmp->SetProperty ( kXMP_NS_ExifEX, "LensModel", xmpValue.c_str() );
		}
	
		haveXMP = xmp->DoesPropertyExist ( kXMP_NS_ExifEX, "LensSpecification" );
		if ( ! haveXMP ) {
			haveXMP = xmp->GetProperty ( kXMP_NS_EXIF_Aux, "LensInfo", &xmpValue, 0 );
			if ( haveXMP ) {
				size_t start, end;
				std::string nextItem;
				for ( start = 0; start < xmpValue.size(); start = end + 1 ) {
					end = xmpValue.find ( ' ', start );
					if ( end == start ) continue;
					if ( end == std::string::npos ) end = xmpValue.size();
					nextItem = xmpValue.substr ( start, (end-start) );
					xmp->AppendArrayItem ( kXMP_NS_ExifEX, "LensSpecification", kXMP_PropArrayIsOrdered, nextItem.c_str() );
				}
			}
		}
	
	#endif
	
	
	ExportTIFF_StandardMappings ( kTIFF_PrimaryIFD, exif, *xmp );
	ExportTIFF_StandardMappings ( kTIFF_ExifIFD, exif, *xmp );
	ExportTIFF_StandardMappings ( kTIFF_GPSInfoIFD, exif, *xmp );
	
	
	TIFF_Manager::Rational altValue;
	bool haveExif = exif->GetTag_Rational ( kTIFF_GPSInfoIFD, kTIFF_GPSAltitude, &altValue );
	if ( haveExif ) {

		bool fixExif = false;

		if ( altValue.denom >> 31 ) {	 
			altValue.denom = -altValue.denom;
			altValue.num = -altValue.num;
			fixExif = true;
		}

		if ( altValue.num >> 31 ) {	 
			exif->SetTag_Byte ( kTIFF_GPSInfoIFD, kTIFF_GPSAltitudeRef, 1 );
			altValue.num = -altValue.num;
			fixExif = true;
		}

		if ( fixExif ) exif->SetTag_Rational ( kTIFF_GPSInfoIFD, kTIFF_GPSAltitude, altValue.num, altValue.denom );

	}

	

	ExportTIFF_LocTextASCII ( *xmp, kXMP_NS_DC, "description",
							  exif, kTIFF_PrimaryIFD, kTIFF_ImageDescription );

	ExportTIFF_EncodedString ( *xmp, kXMP_NS_EXIF, "UserComment",
							   exif, kTIFF_ExifIFD, kTIFF_UserComment, true   );
	

	if ( exif->GetTag ( kTIFF_ExifIFD, kTIFF_DateTimeDigitized, 0 ) ) {
		ExportTIFF_Date ( *xmp, kXMP_NS_XMP, "CreateDate", exif, kTIFF_DateTimeDigitized );
	}

	ExportTIFF_Date ( *xmp, kXMP_NS_EXIF, "DateTimeOriginal", exif, kTIFF_DateTimeOriginal );
	ExportTIFF_Date ( *xmp, kXMP_NS_XMP, "ModifyDate", exif, kTIFF_DateTime );
	

	ExportTIFF_ArrayASCII ( *xmp, kXMP_NS_DC, "creator", exif, kTIFF_PrimaryIFD, kTIFF_Artist );

	ExportTIFF_LocTextASCII ( *xmp, kXMP_NS_DC, "rights", exif, kTIFF_PrimaryIFD, kTIFF_Copyright );
		
	haveXMP = xmp->GetProperty ( kXMP_NS_EXIF, "ExifVersion", &xmpValue, 0 );
	if ( haveXMP && (xmpValue.size() == 4) && (! exif->GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, 0 )) ) {
		exif->SetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, kTIFF_UndefinedType, 4, xmpValue.data() );
	}

	ExportTIFF_PhotographicSensitivity ( xmp, exif );
	
	haveXMP = xmp->DoesPropertyExist ( kXMP_NS_EXIF, "ComponentsConfiguration" );
	if ( haveXMP && (xmp->CountArrayItems ( kXMP_NS_EXIF, "ComponentsConfiguration" ) == 4) &&
		 (! exif->GetTag ( kTIFF_ExifIFD, kTIFF_ComponentsConfiguration, 0 )) ) {
		XMP_Uns8 compConfig[4];
		xmp->GetProperty_Int ( kXMP_NS_EXIF, "ComponentsConfiguration[1]", &int32, 0 );
		compConfig[0] = (XMP_Uns8)int32;
		xmp->GetProperty_Int ( kXMP_NS_EXIF, "ComponentsConfiguration[2]", &int32, 0 );
		compConfig[1] = (XMP_Uns8)int32;
		xmp->GetProperty_Int ( kXMP_NS_EXIF, "ComponentsConfiguration[3]", &int32, 0 );
		compConfig[2] = (XMP_Uns8)int32;
		xmp->GetProperty_Int ( kXMP_NS_EXIF, "ComponentsConfiguration[4]", &int32, 0 );
		compConfig[3] = (XMP_Uns8)int32;
		exif->SetTag ( kTIFF_ExifIFD, kTIFF_ComponentsConfiguration, kTIFF_UndefinedType, 4, &compConfig[0] );
	}
	
	haveXMP = xmp->DoesPropertyExist ( kXMP_NS_EXIF, "Flash" );
	if ( haveXMP && (! exif->GetTag ( kTIFF_ExifIFD, kTIFF_Flash, 0 )) ) {
		XMP_Uns16 binFlash = 0;
		bool field;
		haveXMP = xmp->GetProperty_Bool ( kXMP_NS_EXIF, "Flash/exif:Fired", &field, 0 );
		if ( haveXMP & field ) binFlash |= 0x0001;
		haveXMP = xmp->GetProperty_Int ( kXMP_NS_EXIF, "Flash/exif:Return", &int32, 0 );
		if ( haveXMP ) binFlash |= (int32 & 3) << 1;
		haveXMP = xmp->GetProperty_Int ( kXMP_NS_EXIF, "Flash/exif:Mode", &int32, 0 );
		if ( haveXMP ) binFlash |= (int32 & 3) << 3;
		haveXMP = xmp->GetProperty_Bool ( kXMP_NS_EXIF, "Flash/exif:Function", &field, 0 );
		if ( haveXMP & field ) binFlash |= 0x0020;
		haveXMP = xmp->GetProperty_Bool ( kXMP_NS_EXIF, "Flash/exif:RedEyeMode", &field, 0 );
		if ( haveXMP & field ) binFlash |= 0x0040;
		exif->SetTag_Short ( kTIFF_ExifIFD, kTIFF_Flash, binFlash );
	}
	
	haveXMP = xmp->GetProperty_Int ( kXMP_NS_EXIF, "FileSource", &int32, 0 );
	if ( haveXMP && (! exif->GetTag ( kTIFF_ExifIFD, kTIFF_FileSource, 0 )) ) {
		uns8 = (XMP_Uns8)int32;
		exif->SetTag ( kTIFF_ExifIFD, kTIFF_FileSource, kTIFF_UndefinedType, 1, &uns8 );
	}
	
	haveXMP = xmp->GetProperty_Int ( kXMP_NS_EXIF, "SceneType", &int32, 0 );
	if ( haveXMP && (! exif->GetTag ( kTIFF_ExifIFD, kTIFF_SceneType, 0 )) ) {
		uns8 = (XMP_Uns8)int32;
		exif->SetTag ( kTIFF_ExifIFD, kTIFF_SceneType, kTIFF_UndefinedType, 1, &uns8 );
	}
	

	haveXMP = xmp->GetProperty ( kXMP_NS_EXIF, "GPSVersionID", &xmpValue, 0 );	 
	if ( haveXMP && (! exif->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSVersionID, 0 )) ) {
		XMP_Uns8 gpsID[4];	 
		unsigned int fields[4];	 
		int count = sscanf ( xmpValue.c_str(), "%u.%u.%u.%u", &fields[0], &fields[1], &fields[2], &fields[3] );
		if ( (count == 4) && (fields[0] <= 255) && (fields[1] <= 255) && (fields[2] <= 255) && (fields[3] <= 255) ) {
			gpsID[0] = fields[0]; gpsID[1] = fields[1]; gpsID[2] = fields[2]; gpsID[3] = fields[3];
			exif->SetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSVersionID, kTIFF_ByteType, 4, &gpsID[0] );
		}
	}
	
	ExportTIFF_GPSCoordinate ( *xmp, kXMP_NS_EXIF, "GPSLatitude", exif, kTIFF_GPSInfoIFD, kTIFF_GPSLatitude );

	ExportTIFF_GPSCoordinate ( *xmp, kXMP_NS_EXIF, "GPSLongitude", exif, kTIFF_GPSInfoIFD, kTIFF_GPSLongitude );
	
	ExportTIFF_GPSTimeStamp ( *xmp, kXMP_NS_EXIF, "GPSTimeStamp", exif );

	
	haveXMP = xmp->DoesPropertyExist ( kXMP_NS_EXIF, "GPSDestLatitude" );
	if ( haveXMP && (! exif->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDestLatitude, 0 )) ) {
		ExportTIFF_GPSCoordinate ( *xmp, kXMP_NS_EXIF, "GPSDestLatitude", exif, kTIFF_GPSInfoIFD, kTIFF_GPSDestLatitude );
	}

	haveXMP = xmp->DoesPropertyExist ( kXMP_NS_EXIF, "GPSDestLongitude" );
	if ( haveXMP && (! exif->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDestLongitude, 0 )) ) {
		ExportTIFF_GPSCoordinate ( *xmp, kXMP_NS_EXIF, "GPSDestLongitude", exif, kTIFF_GPSInfoIFD, kTIFF_GPSDestLongitude );
	}

	haveXMP = xmp->GetProperty ( kXMP_NS_EXIF, "GPSProcessingMethod", &xmpValue, 0 );
	if ( haveXMP && (! xmpValue.empty()) && (! exif->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSProcessingMethod, 0 )) ) {
		ExportTIFF_EncodedString ( *xmp, kXMP_NS_EXIF, "GPSProcessingMethod", exif, kTIFF_GPSInfoIFD, kTIFF_GPSProcessingMethod );
	}

	haveXMP = xmp->GetProperty ( kXMP_NS_EXIF, "GPSAreaInformation", &xmpValue, 0 );
	if ( haveXMP && (! xmpValue.empty()) && (! exif->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSAreaInformation, 0 )) ) {
		ExportTIFF_EncodedString ( *xmp, kXMP_NS_EXIF, "GPSAreaInformation", exif, kTIFF_GPSInfoIFD, kTIFF_GPSAreaInformation );
	}
	
}	 
