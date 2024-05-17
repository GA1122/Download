int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	SYSTEMTIME      st;
	FILETIME        ft;
	LARGE_INTEGER   li;
	TIME_ZONE_INFORMATION tzi;
	__int64         t;
	static int      tzflag;

	if (NULL != tp)
	{
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft);
		li.LowPart  = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t  = li.QuadPart;        
		t -= EPOCHFILETIME;      
		t /= 10;                 
		tp->tv_sec  = (long)(t / 1000000);
		tp->tv_usec = (long)(t % 1000000);
	}

	if (NULL != tzp)
	{
		GetTimeZoneInformation(&tzi);

		tzp->tz_minuteswest = tzi.Bias;
		if (tzi.StandardDate.wMonth != 0)
		{
			tzp->tz_minuteswest += tzi.StandardBias * 60;
		}

		if (tzi.DaylightDate.wMonth != 0)
		{
			tzp->tz_dsttime = 1;
		}
		else
		{
			tzp->tz_dsttime = 0;
		}
	}

	return 0;
}
