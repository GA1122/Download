static int inet_abc_len(__be32 addr)
{
	int rc = -1;	 

	if (ipv4_is_zeronet(addr))
		rc = 0;
	else {
		__u32 haddr = ntohl(addr);

		if (IN_CLASSA(haddr))
			rc = 8;
		else if (IN_CLASSB(haddr))
			rc = 16;
		else if (IN_CLASSC(haddr))
			rc = 24;
	}

	return rc;
}