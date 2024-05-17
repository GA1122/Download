ExportTIFF_EncodedString ( const SXMPMeta & xmp, const char * xmpNS, const char * xmpProp,
						   TIFF_Manager * tiff, XMP_Uns8 ifd, XMP_Uns16 id, bool isLangAlt = false )
{
	try {	 

		std::string    xmpValue;
		XMP_OptionBits xmpFlags;

		bool foundXMP = xmp.GetProperty ( xmpNS, xmpProp, &xmpValue, &xmpFlags );
		if ( ! foundXMP ) {
			tiff->DeleteTag ( ifd, id );
			return;
		}

		if ( ! isLangAlt ) {
			if ( ! XMP_PropIsSimple ( xmpFlags ) ) return;	 
		} else {
			if ( ! XMP_ArrayIsAltText ( xmpFlags ) ) return;	 
			bool ok = xmp.GetLocalizedText ( xmpNS, xmpProp, "", "x-default", 0, &xmpValue, 0 );
			if ( ! ok ) return;	 
		}

		XMP_Uns8 encoding = kTIFF_EncodeASCII;
		for ( size_t i = 0; i < xmpValue.size(); ++i ) {
			if ( (XMP_Uns8)xmpValue[i] >= 0x80 ) {
				encoding = kTIFF_EncodeUnicode;
				break;
			}
		}

		tiff->SetTag_EncodedString ( ifd, id, xmpValue.c_str(), encoding );

	} catch ( ... ) {
	}

}	 
