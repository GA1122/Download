dtls1_guess_mtu(unsigned int curr_mtu)
	{
	unsigned int i;

	if ( curr_mtu == 0 )
		return g_probable_mtu[0] ;

	for ( i = 0; i < sizeof(g_probable_mtu)/sizeof(g_probable_mtu[0]); i++)
		if ( curr_mtu > g_probable_mtu[i])
			return g_probable_mtu[i];

	return curr_mtu;
	}
