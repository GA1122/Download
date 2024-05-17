int SafeSock::end_of_message()
{
	int ret_val = FALSE;
	int sent;
        unsigned char * md = 0;

	switch(_coding){
		case stream_encode:
                    if (mdChecker_) {
	    		md = mdChecker_->computeMD();
		    }
                    sent = _outMsg.sendMsg(_sock, _who, 
				           _outMsgID, md);
		    if (md) {
		    	free(md);
		    }
                    _outMsgID.msgNo++;  
                    resetCrypto();

                    if (sent < 0) {
                        return FALSE;
                    } else {
                        return TRUE;
                    }
		case stream_decode:
			if(_msgReady) {
				if(_longMsg) {  
					if(_longMsg->consumed())
						ret_val = TRUE;
					if(_longMsg->prevMsg)
						_longMsg->prevMsg->nextMsg = _longMsg->nextMsg;
					else {
						int index = labs(_longMsg->msgID.ip_addr +
						                 _longMsg->msgID.time +
								     _longMsg->msgID.msgNo) % SAFE_SOCK_HASH_BUCKET_SIZE;
						_inMsgs[index] = _longMsg->nextMsg;
					}
					if(_longMsg->nextMsg)
						_longMsg->nextMsg->prevMsg = _longMsg->prevMsg;
					delete _longMsg;
					_longMsg = NULL;
				} else {  
					if(_shortMsg.consumed())
						ret_val = TRUE;
					_shortMsg.reset();
				}
				_msgReady = false;
			} else { 
				ret_val = TRUE;
			}
            resetCrypto();
			setTriedAuthentication(false);
			break;

		default:
            resetCrypto();
            setTriedAuthentication(false);
			break;
	}
			
	if ( allow_empty_message_flag ) {
		allow_empty_message_flag = FALSE;
		ret_val = TRUE;
	}

	return ret_val;
}
