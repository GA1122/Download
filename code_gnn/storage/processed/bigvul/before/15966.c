ImportArrayTIFF_SShort ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int16 * binPtr = (XMP_Int16*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, ++binPtr ) {

			XMP_Int16 binValue = *binPtr;
			if ( ! nativeEndian ) Flip2 ( &binValue );

			char strValue[20];
			snprintf ( strValue, sizeof(strValue), "%hd", binValue );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue );

		}

	} catch ( ... ) {
	}

}	 
