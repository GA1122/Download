getaddrinfo_merge_err(int e1, int e2)
{
	 
	if (e1 == 0)
		return e2;
	else
		return e1;
}