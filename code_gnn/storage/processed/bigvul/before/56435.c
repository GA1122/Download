http_Setup(struct http *hp, struct ws *ws)
{
	uint16_t shd;
	txt *hd;
	unsigned char *hdf;

	 
	shd = hp->shd;
	hd = hp->hd;
	hdf = hp->hdf;
	memset(hp, 0, sizeof *hp);
	memset(hd, 0, sizeof *hd * shd);
	memset(hdf, 0, sizeof *hdf * shd);
	hp->magic = HTTP_MAGIC;
	hp->ws = ws;
	hp->nhd = HTTP_HDR_FIRST;
	hp->shd = shd;
	hp->hd = hd;
	hp->hdf = hdf;
}