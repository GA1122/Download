ExportTIFF_ArrayASCII ( const SXMPMeta & xmp, const char * xmpNS, const char * xmpProp,
						TIFF_Manager * tiff, XMP_Uns8 ifd, XMP_Uns16 id )
{
	try {	 

		std::string    itemValue, fullValue;
		XMP_OptionBits xmpFlags;

		bool foundXMP = xmp.GetProperty ( xmpNS, xmpProp, 0, &xmpFlags );
		if ( ! foundXMP ) {
			tiff->DeleteTag ( ifd, id );
			return;
		}

		if ( ! XMP_PropIsArray ( xmpFlags ) ) return;	 

		if ( id == kTIFF_Artist ) {
			SXMPUtils::CatenateArrayItems ( xmp, xmpNS, xmpProp, 0, 0,
											(kXMP_PropArrayIsOrdered | kXMPUtil_AllowCommas), &fullValue );
			fullValue += '\x0';	 
		} else {
			size_t count = xmp.CountArrayItems ( xmpNS, xmpProp );
			for ( size_t i = 1; i <= count; ++i ) {	 
				(void) xmp.GetArrayItem ( xmpNS, xmpProp, (XMP_Index)i, &itemValue, &xmpFlags );
				if ( ! XMP_PropIsSimple ( xmpFlags ) ) continue;	 
				fullValue.append ( itemValue );
				fullValue.append ( 1, '\x0' );
			}
		}

		tiff->SetTag ( ifd, id, kTIFF_ASCIIType, (XMP_Uns32)fullValue.size(), fullValue.c_str() );	 

	} catch ( ... ) {
	}

}	 
