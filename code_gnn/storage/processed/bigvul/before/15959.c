ImportArrayTIFF_Double ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		double * binPtr = (double*)tagInfo.dataPtr;
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( size_t i = 0; i < tagInfo.count; ++i, ++binPtr ) {

			double binValue = *binPtr;
			if ( ! nativeEndian ) Flip8 ( &binValue );

			std::string strValue;
			SXMPUtils::ConvertFromFloat ( binValue, "", &strValue );	 

			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue.c_str() );

		}

	} catch ( ... ) {
	}

}	 
