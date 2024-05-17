ImportSingleTIFF_Short ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns16 binValue = GetUns16AsIs ( tagInfo.dataPtr );
		if ( ! nativeEndian ) binValue = Flip2 ( binValue );

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%hu", binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
