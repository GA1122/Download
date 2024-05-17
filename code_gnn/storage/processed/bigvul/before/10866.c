des_cipher(const char *in, char *out, long salt, int count)
{
	uint32		buffer[2];
	uint32		l_out,
				r_out,
				rawl,
				rawr;
	int			retval;

	if (!des_initialised)
		des_init();

	setup_salt(salt);

	 
	memcpy(buffer, in, sizeof(buffer));

	rawl = ntohl(buffer[0]);
	rawr = ntohl(buffer[1]);

	retval = do_des(rawl, rawr, &l_out, &r_out, count);

	buffer[0] = htonl(l_out);
	buffer[1] = htonl(r_out);

	 
	memcpy(out, buffer, sizeof(buffer));

	return (retval);
}
