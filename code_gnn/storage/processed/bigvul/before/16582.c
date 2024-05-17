FileTransfer::GetTransferAck(Stream *s,bool &success,bool &try_again,int &hold_code,int &hold_subcode,MyString &error_desc)
{
	if(!PeerDoesTransferAck) {
		success = true;
		return;
	}

	s->decode();

	ClassAd ad;
	if(!ad.initFromStream(*s) || !s->end_of_message()) {
		char const *ip = NULL;
		if(s->type() == Sock::reli_sock) {
			ip = ((ReliSock *)s)->get_sinful_peer();
		}
		dprintf(D_FULLDEBUG,"Failed to receive download acknowledgment from %s.\n",
				ip ? ip : "(disconnected socket)");
		success = false;
		try_again = true;  
		return;
	}
	int result = -1;
	if(!ad.LookupInteger(ATTR_RESULT,result)) {
		MyString ad_str;
		ad.sPrint(ad_str);
		dprintf(D_ALWAYS,"Download acknowledgment missing attribute: %s.  Full classad: [\n%s]\n",ATTR_RESULT,ad_str.Value());
		success = false;
		try_again = false;
		hold_code = CONDOR_HOLD_CODE_InvalidTransferAck;
		hold_subcode = 0;
		error_desc.sprintf("Download acknowledgment missing attribute: %s",ATTR_RESULT);
		return;
	}
	if(result == 0) {
		success = true;
		try_again = false;
	}
	else if(result > 0) {
		success = false;
		try_again = true;
	}
	else {
		success = false;
		try_again = false;
	}

	if(!ad.LookupInteger(ATTR_HOLD_REASON_CODE,hold_code)) {
		hold_code = 0;
	}
	if(!ad.LookupInteger(ATTR_HOLD_REASON_SUBCODE,hold_subcode)) {
		hold_subcode = 0;
	}
	char *hold_reason_buf = NULL;
	if(ad.LookupString(ATTR_HOLD_REASON,&hold_reason_buf)) {
		error_desc = hold_reason_buf;
		free(hold_reason_buf);
	}
}
