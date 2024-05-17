SafeSock::SafeSock(const SafeSock & orig) 
	: Sock(orig)
{
	init();
	char *buf = NULL;
	buf = orig.serialize();	 
	ASSERT(buf);
	serialize(buf);	 
	delete [] buf;
}
