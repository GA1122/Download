char *ndp_msg_opt_dnssl_domain(struct ndp_msg *msg, int offset,
			       int domain_index)
{
	int i;
	static char buf[256];
	struct __nd_opt_dnssl *dnssl =
			ndp_msg_payload_opts_offset(msg, offset);
	size_t len = dnssl->nd_opt_dnssl_len << 3;  
	char *ptr;

	len -= in_struct_offset(struct __nd_opt_dnssl, nd_opt_dnssl_domains);
	ptr = dnssl->nd_opt_dnssl_domains;

	i = 0;
	while (len > 0) {
		size_t buf_len = 0;
		while (len > 0) {
			uint8_t dom_len = *ptr;

			ptr++;
			len--;
			if (!dom_len)
				break;

			if (dom_len > len)
				return NULL;

			if (buf_len + dom_len + 1 > sizeof(buf))
				return NULL;

			memcpy(buf + buf_len, ptr, dom_len);
			buf[buf_len + dom_len] = '.';
			ptr += dom_len;
			len -= dom_len;
			buf_len += dom_len + 1;
		}
		if (!buf_len)
			break;
		buf[buf_len - 1] = '\0';  
		if (i++ == domain_index)
			return buf;
	}
	return NULL;
}