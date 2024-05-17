void SafeSock::dumpSock()
{
	_condorInMsg *tempMsg;

	dprintf(D_NETWORK, "[In] Long Messages\n");
	for(int i=0; i<SAFE_SOCK_HASH_BUCKET_SIZE; i++) {
		dprintf(D_NETWORK, "\nBucket [%d]\n", i);
		tempMsg = _inMsgs[i];
		while(tempMsg) {
			tempMsg->dumpMsg();
			tempMsg = tempMsg->nextMsg;
		}
	}

	dprintf(D_NETWORK, "\n\n[In] Short Message\n");
	if(_msgReady && _longMsg == NULL)
		_shortMsg.dumpPacket();
	
	dprintf(D_NETWORK, "\n\n[Out] out message\n");
	_outMsg.dumpMsg(_outMsgID);
}
