ImportArrayTIFF_SRational ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
							SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int32 * binPtr = (XMP_Int32*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, binPtr += 2 ) {

			XMP_Int32 binNum   = binPtr[0];
			XMP_Int32 binDenom = binPtr[1];
			if ( ! nativeEndian ) {
				Flip4 ( &binNum );
				Flip4 ( &binDenom );
			}

			char strValue[40];
			snprintf ( strValue, sizeof(strValue), "%ld/%ld", (long)binNum, (long)binDenom );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue );

		}

	} catch ( ... ) {
	}

}	 
