ImportArrayTIFF_Rational ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						   SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns32 * binPtr = (XMP_Uns32*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, binPtr += 2 ) {

			XMP_Uns32 binNum  = GetUns32AsIs ( &binPtr[0] );
			XMP_Uns32 binDenom = GetUns32AsIs ( &binPtr[1] );
			if ( ! nativeEndian ) {
				binNum = Flip4 ( binNum );
				binDenom = Flip4 ( binDenom );
			}

			char strValue[40];
			snprintf ( strValue, sizeof(strValue), "%lu/%lu", (unsigned long)binNum, (unsigned long)binDenom );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue );

		}

	} catch ( ... ) {
	}

}	 
