put_header(struct nethdr *n, int attr, size_t len)
{
	struct netattr *nta = NETHDR_TAIL(n);
	int total_size = NTA_ALIGN(NTA_LENGTH(len));
	int attr_size = NTA_LENGTH(len);
	n->len += total_size;
	nta->nta_attr = htons(attr);
	nta->nta_len = htons(attr_size);
	memset((unsigned char *)nta + attr_size, 0, total_size - attr_size);
	return NTA_DATA(nta);
}
