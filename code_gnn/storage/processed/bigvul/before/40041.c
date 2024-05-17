PGTYPESdate_from_timestamp(timestamp dt)
{
	date		dDate;

	dDate = 0;					 

	if (!TIMESTAMP_NOT_FINITE(dt))
	{
#ifdef HAVE_INT64_TIMESTAMP
		 
		dDate = (dt / USECS_PER_DAY);
#else
		 
		dDate = (dt / (double) SECS_PER_DAY);
#endif
	}

	return dDate;
}
