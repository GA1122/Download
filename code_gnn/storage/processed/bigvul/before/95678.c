void CL_GetPingInfo( int n, char *buf, int buflen ) {
	if ( n < 0 || n >= MAX_PINGREQUESTS || !cl_pinglist[n].adr.port ) {
		if ( buflen ) {
			buf[0] = '\0';
		}
		return;
	}

	Q_strncpyz( buf, cl_pinglist[n].info, buflen );
}