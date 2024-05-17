ExportTIFF_GPSCoordinate ( const SXMPMeta & xmp, const char * xmpNS, const char * xmpProp,
						   TIFF_Manager * tiff, XMP_Uns8 ifd, XMP_Uns16 _id )
{
	XMP_Uns16 refID = _id-1;	 
	XMP_Uns16 locID = _id;

	XMP_Assert ( (locID & 1) == 0 );

	try {	 

		std::string xmpValue;
		XMP_OptionBits xmpFlags;

		bool foundXMP = xmp.GetProperty ( xmpNS, xmpProp, &xmpValue, &xmpFlags );
		if ( ! foundXMP ) {
			tiff->DeleteTag ( ifd, refID );
			tiff->DeleteTag ( ifd, locID );
			return;
		}

		if ( ! XMP_PropIsSimple ( xmpFlags ) ) return;

		const char * chPtr = xmpValue.c_str();	 

		XMP_Uns32 deg=0, minNum=0, minDenom=1, sec=0;


		while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;
		if ( (*chPtr < '0') || (*chPtr > '9') ) return;	 
		for ( ; ('0' <= *chPtr) && (*chPtr <= '9'); ++chPtr ) deg = deg*10 + (*chPtr - '0');
		while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;
		if ( (*chPtr == ',') || (*chPtr == ';') ) ++chPtr;
		while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;


		if ( ('0' <= *chPtr) && (*chPtr <= '9') ) {

			for ( ; ('0' <= *chPtr) && (*chPtr <= '9'); ++chPtr ) minNum = minNum*10 + (*chPtr - '0');
			
			
			if ( *chPtr == '.' ) {

				++chPtr;	 
				for ( ; ('0' <= *chPtr) && (*chPtr <= '9'); ++chPtr ) {
					if ( minDenom > 100*1000 ) continue;	 
					minDenom *= 10;
					minNum = minNum*10 + (*chPtr - '0');
				}

			} else {

				while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;
				if ( (*chPtr == ',') || (*chPtr == ';') ) ++chPtr;
				while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;
				for ( ; ('0' <= *chPtr) && (*chPtr <= '9'); ++chPtr ) sec = sec*10 + (*chPtr - '0');

			}

		}
		

		while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;
		if ( (*chPtr == ',') || (*chPtr == ';') ) ++chPtr;	 
		while ( (*chPtr == ' ') || (*chPtr == '\t') ) ++chPtr;

		char ref[2];
		ref[0] = *chPtr;
		ref[1] = 0;

		if ( ('a' <= ref[0]) && (ref[0] <= 'z') ) ref[0] -= 0x20;
		if ( (ref[0] != 'N') && (ref[0] != 'S') && (ref[0] != 'E') && (ref[0] != 'W') ) return;

		tiff->SetTag ( ifd, refID, kTIFF_ASCIIType, 2, &ref[0] );

		XMP_Uns32 loc[6];
		tiff->PutUns32 ( deg, &loc[0] );
		tiff->PutUns32 ( 1,   &loc[1] );
		tiff->PutUns32 ( minNum,   &loc[2] );
		tiff->PutUns32 ( minDenom, &loc[3] );
		tiff->PutUns32 ( sec, &loc[4] );
		tiff->PutUns32 ( 1,   &loc[5] );

		tiff->SetTag ( ifd, locID, kTIFF_RationalType, 3, &loc[0] );

	} catch ( ... ) {


	}

}	 
