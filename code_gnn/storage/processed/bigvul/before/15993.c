bool PhotoDataUtils::IsValueDifferent ( const IPTC_Manager & newIPTC, const IPTC_Manager & oldIPTC, XMP_Uns8 id )
{
	IPTC_Manager::DataSetInfo newInfo;
	size_t newCount = newIPTC.GetDataSet ( id, &newInfo );
	if ( newCount == 0 ) return false;	 

	IPTC_Manager::DataSetInfo oldInfo;
	size_t oldCount = oldIPTC.GetDataSet ( id, &oldInfo );
	if ( oldCount == 0 ) return true;	 
	
	if ( newCount != oldCount ) return true;

	std::string oldStr, newStr;

	for ( newCount = 0; newCount < oldCount; ++newCount ) {

		if ( ignoreLocalText & (! newIPTC.UsingUTF8()) ) {	 
			(void) newIPTC.GetDataSet ( id, &newInfo, newCount );
			if ( ! ReconcileUtils::IsASCII ( newInfo.dataPtr, newInfo.dataLen ) ) continue;
		}

		(void) newIPTC.GetDataSet_UTF8 ( id, &newStr, newCount );
		(void) oldIPTC.GetDataSet_UTF8 ( id, &oldStr, newCount );
		if ( newStr.size() == 0 ) continue;	 
		if ( newStr != oldStr ) break;

	}

	return ( newCount != oldCount );	 
	
}	 
