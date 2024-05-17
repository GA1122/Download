get_random(void)
{
	static uint64_t lfsr = 0;  
	static uint64_t lcg = 0;   
	uint64_t now = mg_get_current_time_ns();

	if (lfsr == 0) {
		 
		lfsr = mg_get_current_time_ns();
		lcg = mg_get_current_time_ns();
	} else {
		 
		lfsr = (lfsr >> 1)
		       | ((((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1)
		          << 63);
		lcg = lcg * 6364136223846793005LL + 1442695040888963407LL;
	}

	 
	return (lfsr ^ lcg ^ now);
}
