FileTransfer::SendTransferAck(Stream *s,bool success,bool try_again,int hold_code,int hold_subcode,char const *hold_reason)
{
	SaveTransferInfo(success,try_again,hold_code,hold_subcode,hold_reason);

	if(!PeerDoesTransferAck) {
		dprintf(D_FULLDEBUG,"SendTransferAck: skipping transfer ack, because peer does not support it.\n");
		return;
	}

	ClassAd ad;
	int result;
	if(success) {
		result = 0;
	}
	else if(try_again) {
		result = 1;   
	}
	else {
		result = -1;  
	}

	ad.Assign(ATTR_RESULT,result);
	if(!success) {
		ad.Assign(ATTR_HOLD_REASON_CODE,hold_code);
		ad.Assign(ATTR_HOLD_REASON_SUBCODE,hold_subcode);
		if(hold_reason) {
			ad.Assign(ATTR_HOLD_REASON,hold_reason);
		}
	}
	s->encode();
	if(!ad.put(*s) || !s->end_of_message()) {
		char const *ip = NULL;
		if(s->type() == Sock::reli_sock) {
			ip = ((ReliSock *)s)->get_sinful_peer();
		}
		dprintf(D_ALWAYS,"Failed to send download %s to %s.\n",
		        success ? "acknowledgment" : "failure report",
		        ip ? ip : "(disconnected socket)");
	}
}
