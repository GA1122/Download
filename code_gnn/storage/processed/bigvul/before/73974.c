fatal (const char *format, ...)
{
	char *s, buf[256];
	va_list args;
	int len;

	term_reset(STO);
	term_reset(STI);
	
	va_start(args, format);
	len = vsnprintf(buf, sizeof(buf), format, args);
	buf[sizeof(buf) - 1] = '\0';
	va_end(args);
	
	s = "\r\nFATAL: ";
	writen_ni(STO, s, strlen(s));
	writen_ni(STO, buf, len);
	s = "\r\n";
	writen_ni(STO, s, strlen(s));

	 
	sleep(1);

#ifdef UUCP_LOCK_DIR
	uucp_unlock();
#endif
	
	exit(EXIT_FAILURE);
}