ImportSingleTIFF_Float ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		float binValue = *((float*)tagInfo.dataPtr);
		if ( ! nativeEndian ) Flip4 ( &binValue );

		xmp->SetProperty_Float ( xmpNS, xmpProp, binValue );

	} catch ( ... ) {
	}

}	 
