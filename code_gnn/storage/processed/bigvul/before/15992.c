bool PhotoDataUtils::IsValueDifferent ( const TIFF_Manager::TagInfo & exifInfo, const std::string & xmpValue, std::string * exifValue )
{
	if ( exifInfo.dataLen == 0 ) return false;	 

	if ( ReconcileUtils::IsUTF8 ( exifInfo.dataPtr, exifInfo.dataLen ) ) {	 
		exifValue->assign ( (char*)exifInfo.dataPtr, exifInfo.dataLen );
	} else {
		if ( ignoreLocalText ) return false;
		ReconcileUtils::LocalToUTF8 ( exifInfo.dataPtr, exifInfo.dataLen, exifValue );
	}

	return (*exifValue != xmpValue);

}	 
