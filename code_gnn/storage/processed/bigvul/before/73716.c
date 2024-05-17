static const char *encode_puid(aClient *client)
{
	static char buf[HOSTLEN + 20];

	 
	while (!client->local->sasl_cookie)
		client->local->sasl_cookie = getrandom16();

	snprintf(buf, sizeof buf, "%s!0.%d", me.name, client->local->sasl_cookie);

	return buf;
}
