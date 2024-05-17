ImportTIFF_Date ( const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & dateInfo,
				  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	XMP_Uns16 secID = 0;
	switch ( dateInfo.id ) {
		case kTIFF_DateTime          : secID = kTIFF_SubSecTime;			break;
		case kTIFF_DateTimeOriginal  : secID = kTIFF_SubSecTimeOriginal;	break;
		case kTIFF_DateTimeDigitized : secID = kTIFF_SubSecTimeDigitized;	break;
	}
	
	try {	 
	
		if ( (dateInfo.type != kTIFF_ASCIIType) || (dateInfo.count != 20) ) return;

		const char * dateStr = (const char *) dateInfo.dataPtr;
		if ( (dateStr[4] != ':')  || (dateStr[7] != ':')  ||
			 (dateStr[10] != ' ') || (dateStr[13] != ':') || (dateStr[16] != ':') ) return;

		XMP_DateTime binValue;

		binValue.year   = GatherInt ( &dateStr[0], 4 );
		binValue.month  = GatherInt ( &dateStr[5], 2 );
		binValue.day    = GatherInt ( &dateStr[8], 2 );
		if ( (binValue.year != 0) | (binValue.month != 0) | (binValue.day != 0) ) binValue.hasDate = true;

		binValue.hour   = GatherInt ( &dateStr[11], 2 );
		binValue.minute = GatherInt ( &dateStr[14], 2 );
		binValue.second = GatherInt ( &dateStr[17], 2 );
		binValue.nanoSecond = 0;	 
		if ( (binValue.hour != 0) | (binValue.minute != 0) | (binValue.second != 0) ) binValue.hasTime = true;

		binValue.tzSign = 0;	 
		binValue.tzHour = binValue.tzMinute = 0;
		binValue.hasTimeZone = false;	 


		TIFF_Manager::TagInfo secInfo;
		bool found = tiff.GetTag ( kTIFF_ExifIFD, secID, &secInfo );	 

		if ( found && (secInfo.type == kTIFF_ASCIIType) ) {
			const char * fracPtr = (const char *) secInfo.dataPtr;
			binValue.nanoSecond = GatherInt ( fracPtr, secInfo.dataLen );
			size_t digits = 0;
			for ( ; (('0' <= *fracPtr) && (*fracPtr <= '9')); ++fracPtr ) ++digits;
			for ( ; digits < 9; ++digits ) binValue.nanoSecond *= 10;
			if ( binValue.nanoSecond != 0 ) binValue.hasTime = true;
		}

		xmp->SetProperty_Date ( xmpNS, xmpProp, binValue );

	} catch ( ... ) {
	}

}	 
