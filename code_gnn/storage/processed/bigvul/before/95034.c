cmd_http_txresp(CMD_ARGS)
{
	struct http *hp;
	const char *proto = "HTTP/1.1";
	const char *status = "200";
	const char *msg = "Ok";
	int bodylen = 0;
	char *b, *c;
	char *body = NULL, *nullbody;
	int nolen = 0;


	(void)cmd;
	(void)vl;
	CAST_OBJ_NOTNULL(hp, priv, HTTP_MAGIC);
	ONLY_SERVER(hp, av);
	assert(!strcmp(av[0], "txresp"));
	av++;

	VSB_clear(hp->vsb);

	 
	REPLACE(body, "");
	nullbody = body;

	for(; *av != NULL; av++) {
		if (!strcmp(*av, "-proto")) {
			proto = av[1];
			av++;
		} else if (!strcmp(*av, "-status")) {
			status = av[1];
			av++;
		} else if (!strcmp(*av, "-msg")) {
			msg = av[1];
			av++;
			continue;
		} else
			break;
	}

	VSB_printf(hp->vsb, "%s %s %s%s", proto, status, msg, nl);

	for(; *av != NULL; av++) {
		if (!strcmp(*av, "-nolen")) {
			nolen = 1;
		} else if (!strcmp(*av, "-hdr")) {
			VSB_printf(hp->vsb, "%s%s", av[1], nl);
			av++;
		} else
			break;
	}
	for(; *av != NULL; av++) {
		if (!strcmp(*av, "-body")) {
			assert(body == nullbody);
			REPLACE(body, av[1]);

			AN(body);
			av++;
			bodylen = strlen(body);
			for (b = body; *b != '\0'; b++) {
				if(*b == '\\' && b[1] == '0') {
					*b = '\0';
					for(c = b+1; *c != '\0'; c++) {
						*c = c[1];
					}
					b++;
					bodylen--;
				}
			}
		} else if (!strcmp(*av, "-bodylen")) {
			assert(body == nullbody);
			body = synth_body(av[1], 0);
			bodylen = strlen(body);
			av++;
		} else if (!strcmp(*av, "-gzipresidual")) {
			hp->gzipresidual = strtoul(av[1], NULL, 0);
			av++;
		} else if (!strcmp(*av, "-gziplevel")) {
			hp->gziplevel = strtoul(av[1], NULL, 0);
			av++;
		} else if (!strcmp(*av, "-gziplen")) {
			assert(body == nullbody);
			b = synth_body(av[1], 1);
			gzip_body(hp, b, &body, &bodylen);
			VSB_printf(hp->vsb, "Content-Encoding: gzip%s", nl);
			av++;
		} else if (!strcmp(*av, "-gzipbody")) {
			assert(body == nullbody);
			gzip_body(hp, av[1], &body, &bodylen);
			VSB_printf(hp->vsb, "Content-Encoding: gzip%s", nl);
			av++;
		} else
			break;
	}
	if (*av != NULL)
		vtc_log(hp->vl, 0, "Unknown http txresp spec: %s\n", *av);
	if (body != NULL && !nolen)
		VSB_printf(hp->vsb, "Content-Length: %d%s", bodylen, nl);
	VSB_cat(hp->vsb, nl);
	if (body != NULL)
		VSB_bcat(hp->vsb, body, bodylen);
	http_write(hp, 4, "txresp");
}