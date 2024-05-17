UNCURL_EXPORT int32_t uncurl_read_header(struct uncurl_conn *ucc)
{
	int32_t e;

	if (ucc->hin) http_free_header(ucc->hin);
	ucc->hin = NULL;

	char *header = NULL;
	e = uncurl_read_header_(ucc, &header);

	if (e == UNCURL_OK) {
		ucc->hin = http_parse_header(header);
		free(header);
	}

	return e;
}
