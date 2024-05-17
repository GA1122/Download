static uint32_t *rpc_add_credentials(uint32_t *p)
{
	int hl;
	int hostnamelen = 0;

	 

	hl = (hostnamelen + 3) & ~3;

	 
	*p++ = htonl(1);		 
	*p++ = htonl(hl+20);		 
	*p++ = htonl(0);		 
	*p++ = htonl(hostnamelen);	 

	if (hostnamelen & 3)
		*(p + hostnamelen / 4) = 0;  

	   

	p += hl / 4;
	*p++ = 0;			 
	*p++ = 0;			 
	*p++ = 0;			 

	 
	*p++ = 0;			 
	*p++ = 0;			 

	return p;
}
