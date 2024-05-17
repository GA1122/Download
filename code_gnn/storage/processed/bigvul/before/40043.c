PGTYPESdate_mdyjul(int *mdy, date * jdate)
{
	 
	 
	 

	*jdate = (date) (date2j(mdy[2], mdy[0], mdy[1]) - date2j(2000, 1, 1));
}
