static int tls1_check_ec_key(SSL *s,
			unsigned char *curve_id, unsigned char *comp_id)
	{
	const unsigned char *p;
	size_t plen, i;
	int j;
	 
	if (comp_id && s->session->tlsext_ecpointformatlist)
		{
		p = s->session->tlsext_ecpointformatlist;
		plen = s->session->tlsext_ecpointformatlist_length;
		for (i = 0; i < plen; i++, p++)
			{
			if (*comp_id == *p)
				break;
			}
		if (i == plen)
			return 0;
		}
	if (!curve_id)
		return 1;
	 
	for (j = 0; j <= 1; j++)
		{
		tls1_get_curvelist(s, j, &p, &plen);
		for (i = 0; i < plen; i+=2, p+=2)
			{
			if (p[0] == curve_id[0] && p[1] == curve_id[1])
				break;
			}
		if (i == plen)
			return 0;
		 
		if (!s->server)
			break;
		}
	return 1;
	}
