fileTimeToUtc(uint64_t fileTime, time_t *timep, long *ns)
{

	if (fileTime >= EPOC_TIME) {
		fileTime -= EPOC_TIME;
		 
		*timep = (time_t)(fileTime / 10000000);
		 
		*ns = (long)(fileTime % 10000000) * 100;
	} else {
		*timep = 0;
		*ns = 0;
	}
}
