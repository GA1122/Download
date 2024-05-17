ImportArrayTIFF_ASCII ( const TIFF_Manager::TagInfo & tagInfo,
						SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 
	
		TrimTrailingSpaces ( (TIFF_Manager::TagInfo*) &tagInfo );
		if ( tagInfo.dataLen == 0 ) return;	 

		const char * chPtr  = (const char *)tagInfo.dataPtr;
		const char * chEnd  = chPtr + tagInfo.dataLen;
		const bool   hasNul = (chPtr[tagInfo.dataLen-1] == 0);
		const bool   isUTF8 = ReconcileUtils::IsUTF8 ( chPtr, tagInfo.dataLen );

		std::string  strValue;

		if ( (! isUTF8) || (! hasNul) ) {
			if ( isUTF8 ) {
				strValue.assign ( chPtr, tagInfo.dataLen );
			} else {
				if ( ignoreLocalText ) return;
				ReconcileUtils::LocalToUTF8 ( chPtr, tagInfo.dataLen, &strValue );
			}
			chPtr = strValue.c_str();
			chEnd = chPtr + strValue.size();
		}
		
		xmp->DeleteProperty ( xmpNS, xmpProp );	 

		for ( ; chPtr < chEnd; chPtr += (strlen(chPtr) + 1) ) {
			xmp->AppendArrayItem ( xmpNS, xmpProp, kXMP_PropArrayIsOrdered, chPtr );
		}

	} catch ( ... ) {
	}

}	 
