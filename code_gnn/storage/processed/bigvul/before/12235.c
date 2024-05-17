int ssl_fill_hello_random(SSL *s, int server, unsigned char *result, int len)
	{
	int send_time = 0;

	if (len < 4)
		return 0;
	if (server)
		send_time = (s->mode & SSL_MODE_SEND_SERVERHELLO_TIME) != 0;
	else
		send_time = (s->mode & SSL_MODE_SEND_CLIENTHELLO_TIME) != 0;
	if (send_time)
		{
		unsigned long Time = (unsigned long)time(NULL);
		unsigned char *p = result;
		l2n(Time, p);
		return RAND_pseudo_bytes(p, len-4);
		}
	else
		return RAND_pseudo_bytes(result, len);
	}
