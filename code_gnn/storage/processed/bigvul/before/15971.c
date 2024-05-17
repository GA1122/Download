ImportSingleTIFF_Byte ( const TIFF_Manager::TagInfo & tagInfo,
						SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns8 binValue = *((XMP_Uns8*)tagInfo.dataPtr);

		char strValue[20];
		snprintf ( strValue, sizeof(strValue), "%hu", (XMP_Uns16)binValue );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
