get_rtpref(u_int v)
{
	static const char *rtpref_str[] = {
		"medium",		 
		"high",			 
		"rsv",			 
		"low"			 
	};

	return rtpref_str[((v & ND_RA_FLAG_RTPREF_MASK) >> 3) & 0xff];
}
