ImportArrayTIFF_SLong ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int32 * binPtr = (XMP_Int32*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, ++binPtr ) {

			XMP_Int32 binValue = *binPtr;
			if ( ! nativeEndian ) Flip4 ( &binValue );

			char strValue[20];
			snprintf ( strValue, sizeof(strValue), "%ld", (long)binValue );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue );

		}

	} catch ( ... ) {
	}

}	 
