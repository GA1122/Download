void ASF_LegacyManager::ConvertMSDateToISODate ( std::string& source, std::string* dest )
{

	XMP_Int64 creationDate = GetUns64LE ( source.c_str() );
	XMP_Int64 totalSecs = creationDate / (10*1000*1000);
	XMP_Int32 nanoSec = ( ( XMP_Int32) (creationDate - (totalSecs * 10*1000*1000)) ) * 100;

	XMP_Int32 days = (XMP_Int32) (totalSecs / 86400);
	totalSecs -= ( ( XMP_Int64)days * 86400 );

	XMP_Int32 hour = (XMP_Int32) (totalSecs / 3600);
	totalSecs -= ( ( XMP_Int64)hour * 3600 );

	XMP_Int32 minute = (XMP_Int32) (totalSecs / 60);
	totalSecs -= ( ( XMP_Int64)minute * 60 );

	XMP_Int32 second = (XMP_Int32)totalSecs;


	XMP_DateTime date;
	memset ( &date, 0, sizeof ( date ) );

	date.year = 1601;        
	date.month = 1;
	date.day = 1;

	date.day += days;    
	date.hour = hour;
	date.minute = minute;
	date.second = second;
	date.nanoSecond = nanoSec;

	date.hasTimeZone = true;	 
	SXMPUtils::ConvertToUTCTime ( &date );    
	SXMPUtils::ConvertFromDate ( date, dest );    

}
