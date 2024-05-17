match_path(sc_card_t *card, unsigned short int **pathptr, size_t *pathlen,
		int need_info)
{
	unsigned short int	*curptr, *ptr;
	size_t		curlen, len;
	size_t		i;

	curptr = (unsigned short int *) card->cache.current_path.value;
	curlen = card->cache.current_path.len;
	ptr    = *pathptr;
	len    = *pathlen;

	if (curlen < 1 || len < 1)
		return 0;

	 
	if (ptr[0] != GPK_FID_MF || curptr[0] != GPK_FID_MF)
		return 0;

	for (i = 1; i < len && i < curlen; i++) {
		if (ptr[i] != curptr[i])
			break;
	}

	if (len < curlen) {
		 
		if (len == 2)
			goto okay;
		 
		return 0;
	}

	 
	if (i == len && need_info) {
		if (i > 1) {
			*pathptr = ptr + len - 1;
			*pathlen = len - 1;
			return 1;
		}
		 
		return 0;
	}

okay:
	*pathptr = ptr + i;
	*pathlen = len - i;
	return 1;
}
