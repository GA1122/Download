static void Import3WayDateTime ( XMP_Uns16 exifTag, const TIFF_Manager & exif, const IPTC_Manager & iptc,
								 SXMPMeta * xmp, int iptcDigestState, const IPTC_Manager & oldIPTC )
{
	XMP_Uns8  iptcDS;
	XMP_StringPtr xmpNS, xmpProp;
	
	if ( exifTag == kTIFF_DateTimeOriginal ) {
		iptcDS  = kIPTC_DateCreated;
		xmpNS   = kXMP_NS_Photoshop;
		xmpProp = "DateCreated";
	} else if ( exifTag == kTIFF_DateTimeDigitized ) {
		iptcDS  = kIPTC_DigitalCreateDate;
		xmpNS   = kXMP_NS_XMP;
		xmpProp = "CreateDate";
	} else {
		XMP_Throw ( "Unrecognized dateID", kXMPErr_BadParam );
	}

	size_t iptcCount;
	bool haveXMP, haveExif, haveIPTC;	 
	std::string xmpValue, exifValue, iptcValue;
	TIFF_Manager::TagInfo exifInfo;
	IPTC_Manager::DataSetInfo iptcInfo;

	haveXMP   = xmp->GetProperty ( xmpNS, xmpProp, &xmpValue, 0 );
	iptcCount = PhotoDataUtils::GetNativeInfo ( iptc, iptcDS, iptcDigestState, haveXMP, &iptcInfo );
	haveIPTC = (iptcCount > 0);
	XMP_Assert ( (iptcDigestState == kDigestMatches) ? (! haveIPTC) : true );
	haveExif  = (! haveXMP) && (! haveIPTC) && PhotoDataUtils::GetNativeInfo ( exif, kTIFF_ExifIFD, exifTag, &exifInfo );
	XMP_Assert ( (! (haveExif & haveXMP)) & (! (haveExif & haveIPTC)) );

	if ( haveIPTC ) {

		PhotoDataUtils::ImportIPTC_Date ( iptcDS, iptc, xmp );

	} else if ( haveExif && (exifInfo.type == kTIFF_ASCIIType) ) {

	
		TIFF_FileWriter exifFromXMP;
		TIFF_Manager::TagInfo infoFromXMP;

		ExportTIFF_Date ( *xmp, xmpNS, xmpProp, &exifFromXMP, exifTag );
		bool foundFromXMP = exifFromXMP.GetTag ( kTIFF_ExifIFD, exifTag, &infoFromXMP );

		if ( (! foundFromXMP) || (exifInfo.dataLen != infoFromXMP.dataLen) ||
			 (! XMP_LitNMatch ( (char*)exifInfo.dataPtr, (char*)infoFromXMP.dataPtr, exifInfo.dataLen )) ) {
			ImportTIFF_Date ( exif, exifInfo, xmp, xmpNS, xmpProp );
		}

	}

}	 
