http_EstimateWS(const struct http *fm, unsigned how, uint16_t *nhd)
{
	unsigned u, l;

	l = 0;
	*nhd = HTTP_HDR_FIRST;
	CHECK_OBJ_NOTNULL(fm, HTTP_MAGIC);
	for (u = 0; u < fm->nhd; u++) {
		if (fm->hd[u].b == NULL)
			continue;
		if (fm->hdf[u] & HDF_FILTER)
			continue;
#define HTTPH(a, b, c, d, e, f, g) \
		if (((e) & how) && http_IsHdr(&fm->hd[u], (b))) \
			continue;
#include "http_headers.h"
#undef HTTPH
		l += PRNDUP(Tlen(fm->hd[u]) + 1);
		(*nhd)++;
	}
	return (l);
}