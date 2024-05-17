ImportSingleTIFF_Long ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns32 binValue = GetUns32AsIs ( tagInfo.dataPtr );
		if ( ! nativeEndian ) binValue = Flip4 ( binValue );

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%lu", (unsigned long)binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
