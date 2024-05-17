ExportTIFF_GPSTimeStamp ( const SXMPMeta & xmp, const char * xmpNS, const char * xmpProp, TIFF_Manager * tiff )
{
	
	try {	 

		XMP_DateTime binXMP;
		bool foundXMP = xmp.GetProperty_Date ( xmpNS, xmpProp, &binXMP, 0 );
		if ( ! foundXMP ) {
			tiff->DeleteTag ( kTIFF_GPSInfoIFD, kTIFF_GPSTimeStamp );
			tiff->DeleteTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDateStamp );
			return;
		}
		
		SXMPUtils::ConvertToUTCTime ( &binXMP );

		XMP_Uns32 exifTime[6];
		tiff->PutUns32 ( binXMP.hour, &exifTime[0] );
		tiff->PutUns32 ( 1, &exifTime[1] );
		tiff->PutUns32 ( binXMP.minute, &exifTime[2] );
		tiff->PutUns32 ( 1, &exifTime[3] );
		if ( binXMP.nanoSecond == 0 ) {
			tiff->PutUns32 ( binXMP.second, &exifTime[4] );
			tiff->PutUns32 ( 1,   &exifTime[5] );
		} else {
			double fSec = (double)binXMP.second + ((double)binXMP.nanoSecond / kBillion );
			XMP_Uns32 denom = 1000*1000;	 
			TIFF_Manager::TagInfo oldInfo;
			bool hadExif = tiff->GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSTimeStamp, &oldInfo );
			if ( hadExif && (oldInfo.type == kTIFF_RationalType) && (oldInfo.count == 3) ) {
				XMP_Uns32 oldDenom = tiff->GetUns32 ( &(((XMP_Uns32*)oldInfo.dataPtr)[5]) );
				if ( oldDenom != 1 ) denom = oldDenom;
			}
			fSec *= denom;
			while ( fSec > mMaxSec ) { fSec /= 10; denom /= 10; }
			tiff->PutUns32 ( (XMP_Uns32)fSec, &exifTime[4] );
			tiff->PutUns32 ( denom, &exifTime[5] );
		}
		tiff->SetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSTimeStamp, kTIFF_RationalType, 3, &exifTime[0] );

		char exifDate[16];	 
		snprintf ( exifDate, 12, "%04d:%02d:%02d", binXMP.year, binXMP.month, binXMP.day );
		if ( exifDate[10] == 0 ) {	 
			tiff->SetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDateStamp, kTIFF_ASCIIType, 11, exifDate );
		}

	} catch ( ... ) {
	}

}	 
