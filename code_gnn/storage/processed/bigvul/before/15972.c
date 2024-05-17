ImportSingleTIFF_Double ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		double binValue = *((double*)tagInfo.dataPtr);
		if ( ! nativeEndian ) Flip8 ( &binValue );

		xmp->SetProperty_Float ( xmpNS, xmpProp, binValue );	 

	} catch ( ... ) {
	}

}	 
