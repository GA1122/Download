http_DissectResponse(struct worker *w, const struct http_conn *htc,
    struct http *hp)
{
	int j;
	uint16_t retval = 0;
	char *p;


	CHECK_OBJ_NOTNULL(htc, HTTP_CONN_MAGIC);
	CHECK_OBJ_NOTNULL(hp, HTTP_MAGIC);
	hp->logtag = HTTP_Rx;

	if (http_splitline(w, htc->fd, hp, htc,
	    HTTP_HDR_PROTO, HTTP_HDR_STATUS, HTTP_HDR_RESPONSE))
		retval = 503;

	if (retval == 0 && memcmp(hp->hd[HTTP_HDR_PROTO].b, "HTTP/1.", 7))
		retval = 503;

	if (retval == 0 && Tlen(hp->hd[HTTP_HDR_STATUS]) != 3)
		retval = 503;

	if (retval == 0) {
		hp->status = 0;
		p = hp->hd[HTTP_HDR_STATUS].b;
		for (j = 100; j != 0; j /= 10) {
			if (!vct_isdigit(*p)) {
				retval = 503;
				break;
			}
			hp->status += (uint16_t)(j * (*p - '0'));
			p++;
		}
		if (*p != '\0')
			retval = 503;
	}

	if (retval != 0) {
		WSLR(w, SLT_HttpGarbage, htc->fd, htc->rxbuf);
		assert(retval >= 100 && retval <= 999);
		hp->status = retval;
	} else {
		http_ProtoVer(hp);
	}

	if (hp->hd[HTTP_HDR_RESPONSE].b == NULL ||
	    !Tlen(hp->hd[HTTP_HDR_RESPONSE])) {
		 
		hp->hd[HTTP_HDR_RESPONSE].b =
		    TRUST_ME(http_StatusMessage(hp->status));
		hp->hd[HTTP_HDR_RESPONSE].e =
		    strchr(hp->hd[HTTP_HDR_RESPONSE].b, '\0');
	}
	return (retval);
}