ImportTIFF_LocTextASCII ( const TIFF_Manager & tiff, XMP_Uns8 ifd, XMP_Uns16 tagID,
						  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		TIFF_Manager::TagInfo tagInfo;

		bool found = tiff.GetTag ( ifd, tagID, &tagInfo );
		if ( (! found) || (tagInfo.type != kTIFF_ASCIIType) ) return;

		TrimTrailingSpaces ( (TIFF_Manager::TagInfo*) &tagInfo );
		if ( tagInfo.dataLen == 0 ) return;	 

		const char * chPtr  = (const char *)tagInfo.dataPtr;
		const bool   hasNul = (chPtr[tagInfo.dataLen-1] == 0);
		const bool   isUTF8 = ReconcileUtils::IsUTF8 ( chPtr, tagInfo.dataLen );

		if ( isUTF8 && hasNul ) {
			xmp->SetLocalizedText ( xmpNS, xmpProp, "", "x-default", chPtr );
		} else {
			std::string strValue;
			if ( isUTF8 ) {
				strValue.assign ( chPtr, tagInfo.dataLen );
			} else {
				if ( ignoreLocalText ) return;
				ReconcileUtils::LocalToUTF8 ( chPtr, tagInfo.dataLen, &strValue );
			}
			xmp->SetLocalizedText ( xmpNS, xmpProp, "", "x-default", strValue.c_str() );
		}

	} catch ( ... ) {
	}

}	 
