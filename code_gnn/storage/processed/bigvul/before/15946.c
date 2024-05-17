ExportTIFF_Date ( const SXMPMeta & xmp, const char * xmpNS, const char * xmpProp, TIFF_Manager * tiff, XMP_Uns16 mainID )
{
	XMP_Uns8 mainIFD = kTIFF_ExifIFD;
	XMP_Uns16 fracID=0;
	switch ( mainID ) {
		case kTIFF_DateTime : mainIFD = kTIFF_PrimaryIFD; fracID = kTIFF_SubSecTime;	break;
		case kTIFF_DateTimeOriginal  : fracID = kTIFF_SubSecTimeOriginal;	break;
		case kTIFF_DateTimeDigitized : fracID = kTIFF_SubSecTimeDigitized;	break;
	}

	try {	 

		std::string  xmpStr;
		bool foundXMP = xmp.GetProperty ( xmpNS, xmpProp, &xmpStr, 0 );
		if ( ! foundXMP ) {
			tiff->DeleteTag ( mainIFD, mainID );
			tiff->DeleteTag ( kTIFF_ExifIFD, fracID );	 
			return;
		}


		XMP_DateTime xmpBin;
		SXMPUtils::ConvertToDate ( xmpStr.c_str(), &xmpBin );
		
		char buffer[24];
		snprintf ( buffer, sizeof(buffer), "%04d:%02d:%02d %02d:%02d:%02d",	 
				   xmpBin.year, xmpBin.month, xmpBin.day, xmpBin.hour, xmpBin.minute, xmpBin.second );

		size_t xmpLen = xmpStr.size();
		if ( xmpLen < 18 ) {
			buffer[17] = buffer[18] = ' ';
			if ( xmpLen < 15 ) {
				buffer[14] = buffer[15] = ' ';
				if ( xmpLen < 12 ) {
					buffer[11] = buffer[12] = ' ';
					if ( xmpLen < 9 ) {
						buffer[8] = buffer[9] = ' ';
						if ( xmpLen < 6 ) {
							buffer[5] = buffer[6] = ' ';
							if ( xmpLen < 1 ) {
								buffer[0] = buffer[1] = buffer[2] = buffer[3] = ' ';
							}
						}
					}
				}
			}
		}
		
		tiff->SetTag_ASCII ( mainIFD, mainID, buffer );

		if ( xmpBin.nanoSecond == 0 ) {
		
			tiff->DeleteTag ( kTIFF_ExifIFD, fracID );
		
		} else {

			snprintf ( buffer, sizeof(buffer), "%09d", xmpBin.nanoSecond );	 
			for ( size_t i = strlen(buffer)-1; i > 0; --i ) {
				if ( buffer[i] != '0' ) break;
				buffer[i] = 0;	 
			}

			tiff->SetTag_ASCII ( kTIFF_ExifIFD, fracID, buffer );	 

		}

	} catch ( ... ) {
	}

}	 
