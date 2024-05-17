ImportArrayTIFF_Byte ( const TIFF_Manager::TagInfo & tagInfo,
					   SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns8 * binPtr = (XMP_Uns8*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, ++binPtr ) {

			XMP_Uns8 binValue = *binPtr;

			char strValue[20];
			snprintf ( strValue, sizeof(strValue), "%hu", (XMP_Uns16)binValue );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue );

		}

	} catch ( ... ) {
	}

}	 
