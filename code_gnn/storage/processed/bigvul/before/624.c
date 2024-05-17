bool ber_write_OID_String(TALLOC_CTX *mem_ctx, DATA_BLOB *blob, const char *OID)
{
	unsigned int v, v2;
	const char *p = (const char *)OID;
	char *newp;
	int i;

	if (!isdigit(*p)) return false;
	v = strtoul(p, &newp, 10);
	if (newp[0] != '.') return false;
	p = newp + 1;

	if (!isdigit(*p)) return false;
	v2 = strtoul(p, &newp, 10);
	if (newp[0] != '.') return false;
	p = newp + 1;

	 
	*blob = data_blob_talloc(mem_ctx, NULL, strlen(OID));
	if (!blob->data) return false;

	blob->data[0] = 40*v + v2;

	i = 1;
	while (*p) {
		if (!isdigit(*p)) return false;
		v = strtoul(p, &newp, 10);
		if (newp[0] == '.') {
			p = newp + 1;
			 
			if (!*p) return false;
		} else if (newp[0] == '\0') {
			p = newp;
		} else {
			data_blob_free(blob);
			return false;
		}
		if (v >= (1<<28)) blob->data[i++] = (0x80 | ((v>>28)&0x7f));
		if (v >= (1<<21)) blob->data[i++] = (0x80 | ((v>>21)&0x7f));
		if (v >= (1<<14)) blob->data[i++] = (0x80 | ((v>>14)&0x7f));
		if (v >= (1<<7)) blob->data[i++] = (0x80 | ((v>>7)&0x7f));
		blob->data[i++] = (v&0x7f);
	}

	blob->length = i;

	return true;
}
