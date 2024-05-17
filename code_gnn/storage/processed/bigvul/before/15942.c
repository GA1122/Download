ExportArrayTIFF ( TIFF_Manager * tiff, XMP_Uns8 ifd, const TIFF_MappingToXMP & mapInfo, bool nativeEndian,
				  const SXMPMeta & xmp, const char * xmpNS, const char * xmpArray )
{
	XMP_Assert ( (mapInfo.count != 1) && (mapInfo.type != kTIFF_ASCIIType) );
	XMP_Assert ( mapInfo.name[0] != 0 );	 
	XMP_Assert ( (mapInfo.type == kTIFF_ShortType) || (mapInfo.type == kTIFF_RationalType) );
	XMP_Assert ( xmp.DoesPropertyExist ( xmpNS, xmpArray ) );
	
	size_t arraySize = xmp.CountArrayItems ( xmpNS, xmpArray );
	if ( arraySize == 0 ) {
		tiff->DeleteTag ( ifd, mapInfo.id );
		return;
	}
	
	if ( mapInfo.type == kTIFF_ShortType ) {
		
		std::vector<XMP_Uns16> shortVector;
		shortVector.assign ( arraySize, 0 );
		XMP_Uns16 * shortPtr = (XMP_Uns16*) &shortVector[0];
		
		std::string itemPath;
		XMP_Int32 int32;
		XMP_Uns16 uns16;
		for ( size_t i = 1; i <= arraySize; ++i, ++shortPtr ) {
			SXMPUtils::ComposeArrayItemPath ( xmpNS, xmpArray, (XMP_Index)i, &itemPath );
			xmp.GetProperty_Int ( xmpNS, itemPath.c_str(), &int32, 0 );
			uns16 = (XMP_Uns16)int32;
			if ( ! nativeEndian ) uns16 = Flip2 ( uns16 );
			*shortPtr = uns16;
		}
	
		tiff->SetTag ( ifd, mapInfo.id, kTIFF_ShortType, (XMP_Uns32)arraySize, &shortVector[0] );
	
	} else if ( mapInfo.type == kTIFF_RationalType ) {
		
		std::vector<XMP_Uns32> rationalVector;
		rationalVector.assign ( 2*arraySize, 0 );
		XMP_Uns32 * rationalPtr = (XMP_Uns32*) &rationalVector[0];
		
		std::string itemPath, xmpValue;
		XMP_Uns32 num, denom;
		for ( size_t i = 1; i <= arraySize; ++i, rationalPtr += 2 ) {
			SXMPUtils::ComposeArrayItemPath ( xmpNS, xmpArray, (XMP_Index)i, &itemPath );
			bool isPropoerty = xmp.GetProperty ( xmpNS, itemPath.c_str(), &xmpValue, 0 );
			if ( ! isPropoerty ) return;
			bool ok = DecodeRational ( xmpValue.c_str(), &num, &denom );
			if ( ! ok ) return;
			if ( ! nativeEndian ) { num = Flip4 ( num ); denom = Flip4 ( denom ); }
			rationalPtr[0] = num;
			rationalPtr[1] = denom;
		}
	
		tiff->SetTag ( ifd, mapInfo.id, kTIFF_RationalType, (XMP_Uns32)arraySize, &rationalVector[0] );

	}

}	 
