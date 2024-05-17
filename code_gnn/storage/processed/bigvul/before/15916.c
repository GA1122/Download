int ASF_LegacyManager::DaysInMonth ( XMP_Int32 year, XMP_Int32 month )
{

	static short	daysInMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int days = daysInMonth [ month ];
	if ( (month == 2) && IsLeapYear ( year ) ) days += 1;
	
	return days;

}
