static char *_time_stamp_to_str(ut32 timeStamp) {
#ifdef _MSC_VER
	time_t rawtime;
	struct tm *tminfo;
	rawtime = (time_t)timeStamp;
	tminfo = localtime (&rawtime);
	return r_str_trim (strdup (asctime (tminfo)));
#else
	struct my_timezone {
		int tz_minuteswest;      
		int tz_dsttime;          
	} tz;
	struct timeval tv;
	int gmtoff;
	time_t ts = (time_t) timeStamp;
	gettimeofday (&tv, (void*) &tz);
	gmtoff = (int) (tz.tz_minuteswest * 60);  
	ts += (time_t)gmtoff;
	return r_str_trim (strdup (ctime (&ts)));
#endif
}