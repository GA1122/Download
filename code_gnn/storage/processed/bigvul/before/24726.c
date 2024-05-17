struct timespec timespec_trunc(struct timespec t, unsigned gran)
{
	 
	if (gran <= jiffies_to_usecs(1) * 1000) {
		 
	} else if (gran == 1000000000) {
		t.tv_nsec = 0;
	} else {
		t.tv_nsec -= t.tv_nsec % gran;
	}
	return t;
}
