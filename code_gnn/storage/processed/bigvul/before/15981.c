ImportTIFF_CFATable ( const TIFF_Manager::TagInfo & tagInfo, bool nativeEndian,
					  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		const XMP_Uns8 * bytePtr = (XMP_Uns8*)tagInfo.dataPtr;
		const XMP_Uns8 * byteEnd = bytePtr + tagInfo.dataLen;

		XMP_Uns16 columns = *((XMP_Uns16*)bytePtr);
		XMP_Uns16 rows    = *((XMP_Uns16*)(bytePtr+2));
		if ( ! nativeEndian ) {
			columns = Flip2 ( columns );
			rows = Flip2 ( rows );
		}

		char buffer[20];
		std::string arrayPath;

		snprintf ( buffer, sizeof(buffer), "%d", columns );	 
		xmp->SetStructField ( xmpNS, xmpProp, kXMP_NS_EXIF, "Columns", buffer );
		snprintf ( buffer, sizeof(buffer), "%d", rows );	 
		xmp->SetStructField ( xmpNS, xmpProp, kXMP_NS_EXIF, "Rows", buffer );

		bytePtr += 4;	 
		if ( (byteEnd - bytePtr) != (columns * rows) ) goto BadExif;	 

		SXMPUtils::ComposeStructFieldPath ( xmpNS, xmpProp, kXMP_NS_EXIF, "Values", &arrayPath );

		for ( size_t i = (columns * rows); i > 0; --i, ++bytePtr ) {
			snprintf ( buffer, sizeof(buffer), "%hu", (XMP_Uns16)(*bytePtr) );	 
			xmp->AppendArrayItem ( xmpNS, arrayPath.c_str(), kXMP_PropArrayIsOrdered, buffer );
		}

		return;

	BadExif:	 
		xmp->DeleteProperty ( xmpNS, xmpProp );
		return;

	} catch ( ... ) {
	}

}	 
