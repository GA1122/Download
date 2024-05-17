int server_match_spec_complete(struct connection_info *ci)
{
	if (ci->user && ci->host && ci->address)
		return 1;	 
	if (!ci->user && !ci->host && !ci->address)
		return -1;	 
	return 0;	 
}
