ImportSingleTIFF_SByte ( const TIFF_Manager::TagInfo & tagInfo,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Int8 binValue = *((XMP_Int8*)tagInfo.dataPtr);

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%hd", (short)binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
