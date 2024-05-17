SafeSock::~SafeSock()
{
	_condorInMsg *tempMsg, *delMsg;

	for(int i=0; i<SAFE_SOCK_HASH_BUCKET_SIZE; i++) {
		tempMsg = _inMsgs[i];
		while(tempMsg) {
			delMsg = tempMsg;
			tempMsg = tempMsg->nextMsg;
			delete delMsg;
		}
		_inMsgs[i] = NULL;
	}
	close();

    delete mdChecker_;
}
