FileTransfer::setClientSocketTimeout(int timeout)
{
	int old_val = clientSockTimeout;
	clientSockTimeout = timeout;
	return old_val;
}
