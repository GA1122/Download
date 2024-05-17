void ASF_LegacyManager::ConvertISODateToMSDate ( std::string& source, std::string* dest )
{
	XMP_DateTime date;
	SXMPUtils::ConvertToDate ( source, &date );
	SXMPUtils::ConvertToUTCTime ( &date );

	XMP_Int64 creationDate;
	creationDate = date.nanoSecond / 100;
	creationDate += (XMP_Int64 ( date.second) * (10*1000*1000) );
	creationDate += (XMP_Int64 ( date.minute) * 60 * (10*1000*1000) );
	creationDate += (XMP_Int64 ( date.hour) * 3600 * (10*1000*1000) );

	XMP_Int32 days = (date.day - 1);

	--date.month;
	while ( date.month >= 1 ) {
		days += DaysInMonth ( date.year, date.month );
		--date.month;
	}

	--date.year;
	while ( date.year >= 1601 ) {
		days += (IsLeapYear ( date.year) ? 366 : 365 );
		--date.year;
	}

	creationDate += (XMP_Int64 ( days) * 86400 * (10*1000*1000) );

	creationDate = GetUns64LE ( &creationDate );
	dest->assign ( (const char*)&creationDate, 8 );

}
