__u32 sctp_generate_tag(const struct sctp_endpoint *ep)
{
	 
	__u32 x;

	do {
		get_random_bytes(&x, sizeof(__u32));
	} while (x == 0);

	return x;
}
