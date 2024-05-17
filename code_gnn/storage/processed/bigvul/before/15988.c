ImportTIFF_GPSTimeStamp ( const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & timeInfo,
						  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{
	try {	 

		const bool nativeEndian = tiff.IsNativeEndian();

		bool haveDate;
		TIFF_Manager::TagInfo dateInfo;
		haveDate = tiff.GetTag ( kTIFF_GPSInfoIFD, kTIFF_GPSDateStamp, &dateInfo );
		if ( ! haveDate ) haveDate = tiff.GetTag ( kTIFF_ExifIFD, kTIFF_DateTimeOriginal, &dateInfo );
		if ( ! haveDate ) haveDate = tiff.GetTag ( kTIFF_ExifIFD, kTIFF_DateTimeDigitized, &dateInfo );
		if ( ! haveDate ) return;

		const char * dateStr = (const char *) dateInfo.dataPtr;
		if ( ((dateStr[4] != ':') && (dateStr[4] != '-')) || ((dateStr[7] != ':') && (dateStr[7] != '-')) ) return;
		if ( (dateStr[10] != 0)  && (dateStr[10] != ' ') ) return;

		XMP_Uns32 * binPtr = (XMP_Uns32*)timeInfo.dataPtr;
		XMP_Uns32 hourNum   = GetUns32AsIs ( &binPtr[0] );
		XMP_Uns32 hourDenom = GetUns32AsIs ( &binPtr[1] );
		XMP_Uns32 minNum    = GetUns32AsIs ( &binPtr[2] );
		XMP_Uns32 minDenom  = GetUns32AsIs ( &binPtr[3] );
		XMP_Uns32 secNum    = GetUns32AsIs ( &binPtr[4] );
		XMP_Uns32 secDenom  = GetUns32AsIs ( &binPtr[5] );
		if ( ! nativeEndian ) {
			hourNum = Flip4 ( hourNum );
			hourDenom = Flip4 ( hourDenom );
			minNum = Flip4 ( minNum );
			minDenom = Flip4 ( minDenom );
			secNum = Flip4 ( secNum );
			secDenom = Flip4 ( secDenom );
		}

		double fHour, fMin, fSec, fNano, temp;
		fSec  =  (double)secNum / (double)secDenom;
		temp  =  (double)minNum / (double)minDenom;
		fMin  =  (double)((XMP_Uns32)temp);
		fSec  += (temp - fMin) * 60.0;
		temp  =  (double)hourNum / (double)hourDenom;
		fHour =  (double)((XMP_Uns32)temp);
		fSec  += (temp - fHour) * 3600.0;
		temp  =  (double)((XMP_Uns32)fSec);
		fNano =  ((fSec - temp) * (1000.0*1000.0*1000.0)) + 0.5;	 
		fSec  =  temp;

		XMP_DateTime binStamp;
		binStamp.year   = GatherInt ( dateStr, 4 );
		binStamp.month  = GatherInt ( dateStr+5, 2 );
		binStamp.day    = GatherInt ( dateStr+8, 2 );
		binStamp.hour   = (XMP_Int32)fHour;
		binStamp.minute = (XMP_Int32)fMin;
		binStamp.second = (XMP_Int32)fSec;
		binStamp.nanoSecond  = (XMP_Int32)fNano;
		binStamp.hasTimeZone = true;	 
		binStamp.tzSign = kXMP_TimeIsUTC;
		binStamp.tzHour = binStamp.tzMinute = 0;

		xmp->SetProperty_Date ( xmpNS, xmpProp, binStamp );

	} catch ( ... ) {
	}

}	 
