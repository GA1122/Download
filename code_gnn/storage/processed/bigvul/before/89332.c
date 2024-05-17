int format_mac_pxe(char *outbuf, size_t outbuf_len)
{
	uchar ethaddr[6];

	if (outbuf_len < 21) {
		printf("outbuf is too small (%zd < 21)\n", outbuf_len);

		return -EINVAL;
	}

	if (!eth_env_get_enetaddr_by_index("eth", eth_get_dev_index(), ethaddr))
		return -ENOENT;

	sprintf(outbuf, "01-%02x-%02x-%02x-%02x-%02x-%02x",
		ethaddr[0], ethaddr[1], ethaddr[2],
		ethaddr[3], ethaddr[4], ethaddr[5]);

	return 1;
}
