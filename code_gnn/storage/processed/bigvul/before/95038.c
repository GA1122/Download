http_process(struct vtclog *vl, const char *spec, int sock, int *sfd)
{
	struct http *hp;
	char *s, *q;
	int retval;

	(void)sfd;
	ALLOC_OBJ(hp, HTTP_MAGIC);
	AN(hp);
	hp->fd = sock;
	hp->timeout = 15000;
	hp->nrxbuf = 2048*1024;
	hp->vsb = VSB_new_auto();
	hp->rxbuf = malloc(hp->nrxbuf);		 
	hp->sfd = sfd;
	hp->vl = vl;
	hp->gziplevel = 0;
	hp->gzipresidual = -1;
	AN(hp->rxbuf);
	AN(hp->vsb);

	s = strdup(spec);
	q = strchr(s, '\0');
	assert(q > s);
	AN(s);
	parse_string(s, http_cmds, hp, vl);
	retval = hp->fd;
	VSB_delete(hp->vsb);
	free(hp->rxbuf);
	free(hp);
	return (retval);
}