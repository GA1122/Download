void SafeSock::init()
{
	_special_state = safesock_none;

	for(int i=0; i<SAFE_SOCK_HASH_BUCKET_SIZE; i++)
		_inMsgs[i] = NULL;
	_msgReady = false;
	_longMsg = NULL;
	_tOutBtwPkts = SAFE_SOCK_MAX_BTW_PKT_ARVL;

	if(_outMsgID.msgNo == 0) {  

		_outMsgID.ip_addr = mt_random();
		_outMsgID.pid = mt_random() % 65536;  
		_outMsgID.time = mt_random();  
		_outMsgID.msgNo = (unsigned long)get_random_int();
	}

    mdChecker_     = NULL;
}
