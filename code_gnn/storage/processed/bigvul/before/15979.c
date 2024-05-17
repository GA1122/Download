ImportSingleTIFF_SShort ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int16 binValue = *((XMP_Int16*)tagInfo.dataPtr);
		if ( ! nativeEndian ) Flip2 ( &binValue );

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%hd", binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
