PGTYPESdate_dayofweek(date dDate)
{
	 
	return (int) (dDate + date2j(2000, 1, 1) + 1) % 7;
}
