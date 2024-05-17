_warc_rdhdr(struct archive_read *a, struct archive_entry *entry)
{
#define HDR_PROBE_LEN		(12U)
	struct warc_s *w = a->format->data;
	unsigned int ver;
	const char *buf;
	ssize_t nrd;
	const char *eoh;
	 
	warc_string_t fnam;
	 
	warc_type_t ftyp;
	 
	ssize_t cntlen;
	 
	time_t rtime;
	 
	time_t mtime;

start_over:
	 
	buf = __archive_read_ahead(a, HDR_PROBE_LEN, &nrd);

	if (nrd < 0) {
		 
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Bad record header");
		return (ARCHIVE_FATAL);
	} else if (buf == NULL) {
		 
		return (ARCHIVE_EOF);
	}
 	 
	eoh = _warc_find_eoh(buf, nrd);
	if (eoh == NULL) {
		 
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Bad record header");
		return (ARCHIVE_FATAL);
	}
	ver = _warc_rdver(buf, eoh - buf);
	 
	if (ver == 0U) {
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Invalid record version");
		return (ARCHIVE_FATAL);
	} else if (ver < 1200U || ver > 10000U) {
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Unsupported record version: %u.%u",
			ver / 10000, (ver % 10000) / 100);
		return (ARCHIVE_FATAL);
	}
	cntlen = _warc_rdlen(buf, eoh - buf);
	if (cntlen < 0) {
		 
		archive_set_error(
			&a->archive, EINVAL,
			"Bad content length");
		return (ARCHIVE_FATAL);
	}
	rtime = _warc_rdrtm(buf, eoh - buf);
	if (rtime == (time_t)-1) {
		 
		archive_set_error(
			&a->archive, EINVAL,
			"Bad record time");
		return (ARCHIVE_FATAL);
	}

	 
	a->archive.archive_format = ARCHIVE_FORMAT_WARC;
	if (ver != w->pver) {
		 
		archive_string_sprintf(&w->sver,
			"WARC/%u.%u", ver / 10000, (ver % 10000) / 100);
		 
		w->pver = ver;
	}
	 
	ftyp = _warc_rdtyp(buf, eoh - buf);
	 
	w->cntlen = cntlen;
	w->cntoff = 0U;
	mtime = 0; 

	switch (ftyp) {
	case WT_RSRC:
	case WT_RSP:
		 
		fnam = _warc_rduri(buf, eoh - buf);
		 
		if (fnam.len == 0 || fnam.str[fnam.len - 1] == '/') {
			 
			fnam.len = 0U;
			fnam.str = NULL;
			break;
		}
		 
		if (fnam.len + 1U > w->pool.len) {
			w->pool.len = ((fnam.len + 64U) / 64U) * 64U;
			w->pool.str = realloc(w->pool.str, w->pool.len);
		}
		memcpy(w->pool.str, fnam.str, fnam.len);
		w->pool.str[fnam.len] = '\0';
		 
		fnam.str = w->pool.str;

		 
		if ((mtime = _warc_rdmtm(buf, eoh - buf)) == (time_t)-1) {
			mtime = rtime;
		}
		break;
	default:
		fnam.len = 0U;
		fnam.str = NULL;
		break;
	}

	 
	__archive_read_consume(a, eoh - buf);

	switch (ftyp) {
	case WT_RSRC:
	case WT_RSP:
		if (fnam.len > 0U) {
			 
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_copy_pathname(entry, fnam.str);
			archive_entry_set_size(entry, cntlen);
			archive_entry_set_perm(entry, 0644);
			 
			archive_entry_set_ctime(entry, rtime, 0L);
			archive_entry_set_mtime(entry, mtime, 0L);
			break;
		}
		 
	default:
		 
		_warc_skip(a);
		goto start_over;
	}
	return (ARCHIVE_OK);
}
