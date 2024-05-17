ImportTIFF_Flash ( const TIFF_Manager::TagInfo & tagInfo, bool nativeEndian,
				   SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		XMP_Uns16 binValue = GetUns16AsIs ( tagInfo.dataPtr );
		if ( ! nativeEndian ) binValue = Flip2 ( binValue );

		bool fired    = (bool)(binValue & 1);	 
		int  rtrn     = (binValue >> 1) & 3;
		int  mode     = (binValue >> 3) & 3;
		bool function = (bool)((binValue >> 5) & 1);
		bool redEye   = (bool)((binValue >> 6) & 1);

		static const char * sTwoBits[] = { "0", "1", "2", "3" };

		xmp->SetStructField ( kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "Fired", (fired ? kXMP_TrueStr : kXMP_FalseStr) );
		xmp->SetStructField ( kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "Return", sTwoBits[rtrn] );
		xmp->SetStructField ( kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "Mode", sTwoBits[mode] );
		xmp->SetStructField ( kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "Function", (function ? kXMP_TrueStr : kXMP_FalseStr) );
		xmp->SetStructField ( kXMP_NS_EXIF, "Flash", kXMP_NS_EXIF, "RedEyeMode", (redEye ? kXMP_TrueStr : kXMP_FalseStr) );

	} catch ( ... ) {
	}

}	 
