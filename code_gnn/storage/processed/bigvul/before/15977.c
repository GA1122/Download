ImportSingleTIFF_SLong ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int32 binValue = *((XMP_Int32*)tagInfo.dataPtr);
		if ( ! nativeEndian ) Flip4 ( &binValue );

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%ld", (long)binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
