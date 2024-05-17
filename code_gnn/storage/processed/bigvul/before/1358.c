static uint32_t *rpc_add_credentials(uint32_t *p)
{
	 

	 
	*p++ = hton32(1);		 
	*p++ = hton32(20);		 
	*p++ = hton32(0);		 
	*p++ = hton32(0);		 
	 

	*p++ = 0;			 
	*p++ = 0;			 
	*p++ = 0;			 

	 
	*p++ = 0;			 
	*p++ = 0;			 

	return p;
}
