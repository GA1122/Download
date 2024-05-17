http_CollectHdr(struct http *hp, const char *hdr)
{
	unsigned u, v, ml, f = 0, x;
	char *b = NULL, *e = NULL;

	for (u = HTTP_HDR_FIRST; u < hp->nhd; u++) {
		while (u < hp->nhd && http_IsHdr(&hp->hd[u], hdr)) {
			Tcheck(hp->hd[u]);
			if (f == 0) {
				 
				f = u;
				break;
			}
			if (b == NULL) {
				 
				ml = WS_Reserve(hp->ws, 0);
				b = hp->ws->f;
				e = b + ml;
				x = Tlen(hp->hd[f]);
				if (b + x < e) {
					memcpy(b, hp->hd[f].b, x);
					b += x;
				} else
					b = e;
			}

			AN(b);
			AN(e);

			 
			if (b < e)
				*b++ = ',';
			x = Tlen(hp->hd[u]) - *hdr;
			if (b + x < e) {
				memcpy(b, hp->hd[u].b + *hdr, x);
				b += x;
			} else
				b = e;

			 
			for (v = u; v < hp->nhd - 1; v++)
				hp->hd[v] = hp->hd[v + 1];
			hp->nhd--;
		}

	}
	if (b == NULL)
		return;
	AN(e);
	if (b >= e) {
		WS_Release(hp->ws, 0);
		return;
	}
	*b = '\0';
	hp->hd[f].b = hp->ws->f;
	hp->hd[f].e = b;
	WS_ReleaseP(hp->ws, b + 1);
}