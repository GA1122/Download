fetch_token(netdissect_options *ndo, const u_char *pptr, u_int idx, u_int len,
    u_char *tbuf, size_t tbuflen)
{
	size_t toklen = 0;

	for (; idx < len; idx++) {
		if (!ND_TTEST(*(pptr + idx))) {
			 
			return (0);
		}
		if (!isascii(*(pptr + idx))) {
			 
			return (0);
		}
		if (isspace(*(pptr + idx))) {
			 
			break;
		}
		if (!isprint(*(pptr + idx))) {
			 
			return (0);
		}
		if (toklen + 2 > tbuflen) {
			 
			return (0);
		}
		tbuf[toklen] = *(pptr + idx);
		toklen++;
	}
	if (toklen == 0) {
		 
		return (0);
	}
	tbuf[toklen] = '\0';

	 
	for (; idx < len; idx++) {
		if (!ND_TTEST(*(pptr + idx))) {
			 
			break;
		}
		if (*(pptr + idx) == '\r' || *(pptr + idx) == '\n') {
			 
			break;
		}
		if (!isascii(*(pptr + idx)) || !isprint(*(pptr + idx))) {
			 
			break;
		}
		if (!isspace(*(pptr + idx))) {
			 
			break;
		}
	}
	return (idx);
}
