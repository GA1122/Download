std::string TimeToString(const time_t *ltime, const _eTimeFormat format)
{
	struct tm timeinfo;
	struct timeval tv;
	std::stringstream sstr;
	if (ltime == NULL)  
	{
#ifdef CLOCK_REALTIME
		struct timespec ts;
		if (!clock_gettime(CLOCK_REALTIME, &ts))
		{
			tv.tv_sec = ts.tv_sec;
			tv.tv_usec = ts.tv_nsec / 1000;
		}
		else
#endif
			gettimeofday(&tv, NULL);
#ifdef WIN32
		time_t tv_sec = tv.tv_sec;
		localtime_r(&tv_sec, &timeinfo);
#else
		localtime_r(&tv.tv_sec, &timeinfo);
#endif
	}
	else
		localtime_r(ltime, &timeinfo);

	if (format > TF_Time)
	{
		sstr << (timeinfo.tm_year + 1900) << "-"
		<< std::setw(2)	<< std::setfill('0') << (timeinfo.tm_mon + 1) << "-"
		<< std::setw(2) << std::setfill('0') << timeinfo.tm_mday;
	}

	if (format != TF_Date)
	{
		if (format > TF_Time)
			sstr << " ";
		sstr
		<< std::setw(2) << std::setfill('0') << timeinfo.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << timeinfo.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << timeinfo.tm_sec;
	}

	if (format > TF_DateTime && ltime == NULL)
		sstr << "." << std::setw(3) << std::setfill('0') << ((int)tv.tv_usec / 1000);

	return sstr.str();
}