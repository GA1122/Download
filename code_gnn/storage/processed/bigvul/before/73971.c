do_map (char *b, int map, char c)
{
	int n;

	switch (c) {
	case '\x7f':
		 
		if ( map & M_DELBS ) {
			b[0] = '\x08'; n = 1;
		} else {
			b[0] = c; n = 1;
		}
		break;
	case '\x08':
		 
		if ( map & M_BSDEL ) {
			b[0] = '\x7f'; n = 1;
		} else {
			b[0] = c; n = 1;
		}
		break;
	case '\x0d':
		 
		if ( map & M_CRLF ) {
			b[0] = '\x0a'; n = 1;
		} else if ( map & M_CRCRLF ) {
			b[0] = '\x0d'; b[1] = '\x0a'; n = 2;
		} else if ( map & M_IGNCR ) {
			n = 0;
		} else {
			b[0] = c; n = 1;
		}
		break;
	case '\x0a':
		 
		if ( map & M_LFCR ) {
			b[0] = '\x0d'; n = 1;
		} else if ( map & M_LFCRLF ) {
			b[0] = '\x0d'; b[1] = '\x0a'; n = 2;
		} else if ( map & M_IGNLF ) {
			n = 0;
		} else {
			b[0] = c; n = 1;
		}
		break;
	default:
		b[0] = c; n = 1;
		break;
	}

	return n;
}