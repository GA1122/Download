ImportSingleTIFF_SRational ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
							 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

#if SUNOS_SPARC || XMP_IOS_ARM
        XMP_Uns32  binPtr[2];
        memcpy(&binPtr, tagInfo.dataPtr, sizeof(XMP_Uns32)*2);
#else
	XMP_Uns32 * binPtr = (XMP_Uns32*)tagInfo.dataPtr;
#endif  
		XMP_Int32 binNum   = GetUns32AsIs ( &binPtr[0] );
		XMP_Int32 binDenom = GetUns32AsIs ( &binPtr[1] );
		if ( ! nativeEndian ) {
			Flip4 ( &binNum );
			Flip4 ( &binDenom );
		}

		char strValue[40];
		snprintf ( strValue, sizeof(strValue), "%ld/%ld", (unsigned long)binNum, (unsigned long)binDenom );	 

		xmp->SetProperty ( xmpNS, xmpProp, strValue );

	} catch ( ... ) {
	}

}	 
