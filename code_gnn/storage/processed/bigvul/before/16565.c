FileTransfer::Continue()
{
	int result = TRUE;	 

	if (ActiveTransferTid != -1 ) {
		ASSERT( daemonCore );
		result = daemonCore->Continue_Thread(ActiveTransferTid);
	}

	return result;
}
