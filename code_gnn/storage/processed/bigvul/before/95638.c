void CL_AddToLimboChat( const char *str ) {
	int len = 0;
	char *p;
	int i;

	cl.limboChatPos = LIMBOCHAT_HEIGHT - 1;

	for ( i = cl.limboChatPos; i > 0; i-- ) {
		strcpy( cl.limboChatMsgs[i], cl.limboChatMsgs[i - 1] );
	}

	p = cl.limboChatMsgs[0];
	*p = 0;

	while ( *str ) {
		if ( len > LIMBOCHAT_WIDTH - 1 ) {
			break;
		}

		if ( Q_IsColorString( str ) ) {
			*p++ = *str++;
			*p++ = *str++;
			continue;
		}
		*p++ = *str++;
		len++;
	}
	*p = 0;
}
