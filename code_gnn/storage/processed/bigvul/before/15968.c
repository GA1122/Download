ImportConversionTable ( const TIFF_Manager::TagInfo & tagInfo, bool nativeEndian,
					    SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	const bool isSigned = (tagInfo.id == kTIFF_OECF);
	XMP_Assert ( (tagInfo.id == kTIFF_OECF) || (tagInfo.id == kTIFF_SpatialFrequencyResponse) );
	
	xmp->DeleteProperty ( xmpNS, xmpProp );
	
	try {	 

		const XMP_Uns8 * bytePtr = (XMP_Uns8*)tagInfo.dataPtr;
		const XMP_Uns8 * byteEnd = bytePtr + tagInfo.dataLen;

		XMP_Uns16 columns = *((XMP_Uns16*)bytePtr);
		XMP_Uns16 rows    = *((XMP_Uns16*)(bytePtr+2));
		if ( ! nativeEndian ) {
			columns = Flip2 ( columns );
			rows = Flip2 ( rows );
		}

		char buffer[40];

		snprintf ( buffer, sizeof(buffer), "%d", columns );	 
		xmp->SetStructField ( xmpNS, xmpProp, kXMP_NS_EXIF, "Columns", buffer );
		snprintf ( buffer, sizeof(buffer), "%d", rows );	 
		xmp->SetStructField ( xmpNS, xmpProp, kXMP_NS_EXIF, "Rows", buffer );

		std::string arrayPath;

		SXMPUtils::ComposeStructFieldPath ( xmpNS, xmpProp, kXMP_NS_EXIF, "Names", &arrayPath );

		bytePtr += 4;	 
		for ( size_t i = columns; i > 0; --i ) {
			size_t nameLen = strlen((XMP_StringPtr)bytePtr) + 1;	 
			if ( (bytePtr + nameLen) > byteEnd ) XMP_Throw ( "OECF-SFR name overflow", kXMPErr_BadValue );
			if ( ! ReconcileUtils::IsUTF8 ( bytePtr, nameLen ) ) XMP_Throw ( "OECF-SFR name error", kXMPErr_BadValue );
			xmp->AppendArrayItem ( xmpNS, arrayPath.c_str(), kXMP_PropArrayIsOrdered, (XMP_StringPtr)bytePtr );
			bytePtr += nameLen;
		}

		if ( (byteEnd - bytePtr) != (8 * columns * rows) ) XMP_Throw ( "OECF-SFR data overflow", kXMPErr_BadValue );
		SXMPUtils::ComposeStructFieldPath ( xmpNS, xmpProp, kXMP_NS_EXIF, "Values", &arrayPath );

		XMP_Uns32 * binPtr = (XMP_Uns32*)bytePtr;
		for ( size_t i = (columns * rows); i > 0; --i, binPtr += 2 ) {

			XMP_Uns32 binNum   = binPtr[0];
			XMP_Uns32 binDenom = binPtr[1];
			if ( ! nativeEndian ) {
				Flip4 ( &binNum );
				Flip4 ( &binDenom );
			}

			if ( (binDenom == 0) && (binNum != 0) ) XMP_Throw ( "OECF-SFR data overflow", kXMPErr_BadValue );
			if ( isSigned ) {
				snprintf ( buffer, sizeof(buffer), "%ld/%ld", (long)binNum, (long)binDenom );	 
			} else {
				snprintf ( buffer, sizeof(buffer), "%lu/%lu", (unsigned long)binNum, (unsigned long)binDenom );	 
			}

			xmp->AppendArrayItem ( xmpNS, arrayPath.c_str(), kXMP_PropArrayIsOrdered, buffer );

		}

		return;

	} catch ( ... ) {
		xmp->DeleteProperty ( xmpNS, xmpProp );
	}

}	 
