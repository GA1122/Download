http_CopyHome(struct worker *w, int fd, const struct http *hp)
{
	unsigned u, l;
	char *p;

	for (u = 0; u < hp->nhd; u++) {
		if (hp->hd[u].b == NULL)
			continue;
		if (hp->hd[u].b >= hp->ws->s && hp->hd[u].e <= hp->ws->e) {
			WSLH(w, fd, hp, u);
			continue;
		}
		l = Tlen(hp->hd[u]);
		p = WS_Alloc(hp->ws, l + 1);
		if (p != NULL) {
			WSLH(w, fd, hp, u);
			memcpy(p, hp->hd[u].b, l + 1L);
			hp->hd[u].b = p;
			hp->hd[u].e = p + l;
		} else {
			 
			VSC_C_main->losthdr++;
			WSLR(w, SLT_LostHeader, fd, hp->hd[u]);
			hp->hd[u].b = NULL;
			hp->hd[u].e = NULL;
		}
	}
}