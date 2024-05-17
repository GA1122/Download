ImportTIFF_GPSCoordinate ( const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & posInfo,
						   SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		const bool nativeEndian = tiff.IsNativeEndian();
		
		if ( (posInfo.type != kTIFF_RationalType) || (posInfo.count == 0) ) return;

		XMP_Uns16 refID = posInfo.id - 1;	 
		TIFF_Manager::TagInfo refInfo;
		bool found = tiff.GetTag ( kTIFF_GPSInfoIFD, refID, &refInfo );
		if ( (! found) || (refInfo.count == 0) ) return;
		char ref = *((char*)refInfo.dataPtr);
		if ( (ref != 'N') && (ref != 'S') && (ref != 'E') && (ref != 'W') ) return;

		XMP_Uns32 * binPtr = (XMP_Uns32*)posInfo.dataPtr;
		XMP_Uns32 degNum = 0, degDenom = 1;	 
		XMP_Uns32 minNum = 0, minDenom = 1;
		XMP_Uns32 secNum = 0, secDenom = 1;
		if ( ! nativeEndian ) {
			degDenom = Flip4 ( degDenom );	 
			minDenom = Flip4 ( minDenom );
			secDenom = Flip4 ( secDenom );
		}
		
		degNum   = GetUns32AsIs ( &binPtr[0] );
		degDenom = GetUns32AsIs ( &binPtr[1] );

		if ( posInfo.count >= 2 ) {
			minNum   = GetUns32AsIs ( &binPtr[2] );
			minDenom = GetUns32AsIs ( &binPtr[3] );
			if ( posInfo.count >= 3 ) {
				secNum   = GetUns32AsIs ( &binPtr[4] );
				secDenom = GetUns32AsIs ( &binPtr[5] );
			}
		}
		
		if ( ! nativeEndian ) {
			degNum = Flip4 ( degNum );
			degDenom = Flip4 ( degDenom );
			minNum = Flip4 ( minNum );
			minDenom = Flip4 ( minDenom );
			secNum = Flip4 ( secNum );
			secDenom = Flip4 ( secDenom );
		}


		char buffer[40];

		if ( (degDenom == 1) && (minDenom == 1) && (secDenom == 1) ) {

			snprintf ( buffer, sizeof(buffer), "%lu,%lu,%lu%c", (unsigned long)degNum, (unsigned long)minNum, (unsigned long)secNum, ref );	 

		} else if ( (degDenom == 0 && degNum != 0) || (minDenom == 0 && minNum != 0) || (secDenom == 0 && secNum != 0) ) { 

			return;  

		} else {

			XMP_Uns32 maxDenom = degDenom;
			if ( minDenom > maxDenom ) maxDenom = minDenom;
			if ( secDenom > maxDenom ) maxDenom = secDenom;

			int fracDigits = 1;
			while ( maxDenom > 10 ) { ++fracDigits; maxDenom = maxDenom/10; }


			double degrees, minutes;

			if ( degDenom == 0 && degNum == 0 ) {
				degrees = 0;
			} else {
				degrees = (double)( (XMP_Uns32)((double)degNum / (double)degDenom) );	 
			}

			if ( minDenom == 0 && minNum == 0 ) {
				minutes = 0;
			} else {
				double temp = 0;
				if( degrees != 0 ) temp = ((double)degNum / (double)degDenom) - degrees;

				minutes = (temp * 60.0) + ((double)minNum / (double)minDenom);
			}

			if ( secDenom != 0 && secNum != 0 ) {
				minutes += ((double)secNum / (double)secDenom) / 60.0;
			}

			snprintf ( buffer, sizeof(buffer), "%.0f,%.*f%c", degrees, fracDigits, minutes, ref );	 

		}

		xmp->SetProperty ( xmpNS, xmpProp, buffer );

	} catch ( ... ) {
	}

}	 
