ImportSingleTIFF_ASCII ( const TIFF_Manager::TagInfo & tagInfo,
						 SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 
	
		TrimTrailingSpaces ( (TIFF_Manager::TagInfo*) &tagInfo );
		if ( tagInfo.dataLen == 0 ) return;	 

		const char * chPtr  = (const char *)tagInfo.dataPtr;
		const bool   hasNul = !tagInfo.dataLen || !chPtr || (chPtr[tagInfo.dataLen-1] == 0);
		const bool   isUTF8 = ReconcileUtils::IsUTF8 ( chPtr, tagInfo.dataLen );

		if ( isUTF8 && hasNul ) {
			xmp->SetProperty ( xmpNS, xmpProp, chPtr );
		} else {
			std::string strValue;
			if ( isUTF8 ) {
				strValue.assign ( chPtr, tagInfo.dataLen );
			} else {
				if ( ignoreLocalText ) return;
				ReconcileUtils::LocalToUTF8 ( chPtr, tagInfo.dataLen, &strValue );
			}
			xmp->SetProperty ( xmpNS, xmpProp, strValue.c_str() );
		}

	} catch ( ... ) {
	}

}	 
