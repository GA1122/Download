ImportTIFF_CheckStandardMapping ( const TIFF_Manager::TagInfo & tagInfo, const TIFF_MappingToXMP & mapInfo )
{
	XMP_Assert ( (kTIFF_ByteType <= tagInfo.type) && (tagInfo.type <= kTIFF_LastType) );
	XMP_Assert ( mapInfo.type <= kTIFF_LastType );

	if ( (tagInfo.type < kTIFF_ByteType) || (tagInfo.type > kTIFF_LastType) ) return false;

	if ( tagInfo.type != mapInfo.type ) {
		if ( kTIFF_IsIntegerType[mapInfo.type] ) {
			if ( ! kTIFF_IsIntegerType[tagInfo.type] ) return false;
		} else if ( kTIFF_IsRationalType[mapInfo.type] ) {
			if ( ! kTIFF_IsRationalType[tagInfo.type] ) return false;
		} else if ( kTIFF_IsFloatType[mapInfo.type] ) {
			if ( ! kTIFF_IsFloatType[tagInfo.type] ) return false;
		} else {
			return false;
		}
	}

	if ( (tagInfo.count != mapInfo.count) &&	 
		 (mapInfo.count == 1) ) return false;	 

	return true;

}	 
