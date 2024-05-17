SafeSock::peek_end_of_message()
{
	if(_msgReady) {
		if(_longMsg) {  
			if(_longMsg->consumed()) {
				return true;
			}
		} else {  
			if(_shortMsg.consumed())
				return true;
		}
	}
	return false;
}
