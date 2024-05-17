tcp_sendf (server *serv, const char *fmt, ...)
{
	va_list args;
	 
	static char send_buf[1540];	 
	int len;

	va_start (args, fmt);
	len = vsnprintf (send_buf, sizeof (send_buf) - 1, fmt, args);
	va_end (args);

	send_buf[sizeof (send_buf) - 1] = '\0';
	if (len < 0 || len > (sizeof (send_buf) - 1))
		len = strlen (send_buf);

	tcp_send_len (serv, send_buf, len);
}
