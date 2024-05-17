bool ASF_LegacyManager::IsLeapYear ( long year )
{

	if ( year < 0 ) year = -year + 1;		 

	if ( (year % 4) != 0 ) return false;	 
	if ( (year % 100) != 0 ) return true;	 
	if ( (year % 400) == 0 ) return true;	 

	return false;							 

}
