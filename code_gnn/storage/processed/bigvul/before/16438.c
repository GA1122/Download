CStarter::updateX509Proxy( int cmd, Stream* s )
{
	ASSERT(s);
	ReliSock* rsock = (ReliSock*)s;
	ASSERT(jic);
	return jic->updateX509Proxy(cmd,rsock) ? TRUE : FALSE;
}
