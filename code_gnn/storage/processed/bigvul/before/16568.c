FileTransfer::DoObtainAndSendTransferGoAhead(DCTransferQueue &xfer_queue,bool downloading,Stream *s,char const *full_fname,bool &go_ahead_always,bool &try_again,int &hold_code,int &hold_subcode,MyString &error_desc)
{
	ClassAd msg;
	int go_ahead = GO_AHEAD_UNDEFINED;
	int alive_interval = 0;
	time_t last_alive = time(NULL);
	const int alive_slop = 20;
	int min_timeout = 300;

	s->decode();
	if( !s->get(alive_interval) || !s->end_of_message() ) {
		error_desc.sprintf("ObtainAndSendTransferGoAhead: failed on alive_interval before GoAhead");
		return false;
	}

	if( Sock::get_timeout_multiplier() > 0 ) {
		min_timeout *= Sock::get_timeout_multiplier();
	}

	int timeout = alive_interval;
	if( timeout < min_timeout ) {
		timeout = min_timeout;

		msg.Assign(ATTR_TIMEOUT,timeout);
		msg.Assign(ATTR_RESULT,go_ahead);

		s->encode();
		if( !msg.put(*s) || !s->end_of_message() ) {
			error_desc.sprintf("Failed to send GoAhead new timeout message.");
		}
	}
	ASSERT( timeout > alive_slop );
	timeout -= alive_slop;

	if( !xfer_queue.RequestTransferQueueSlot(downloading,full_fname,m_jobid.Value(),timeout,error_desc) )
	{
		go_ahead = GO_AHEAD_FAILED;
	}

	while(1) {
		if( go_ahead == GO_AHEAD_UNDEFINED ) {
			timeout = alive_interval - (time(NULL) - last_alive) - alive_slop;
			if( timeout < min_timeout ) timeout = min_timeout;
			bool pending = true;
			if( xfer_queue.PollForTransferQueueSlot(timeout,pending,error_desc) )
			{
				if( xfer_queue.GoAheadAlways( downloading ) ) {
					go_ahead = GO_AHEAD_ALWAYS;
				}
				else {
					go_ahead = GO_AHEAD_ONCE;
				}
			}
			else if( !pending ) {
				go_ahead = GO_AHEAD_FAILED;
			}
		}

		char const *ip = s->peer_ip_str();
		char const *go_ahead_desc = "";
		if( go_ahead < 0 ) go_ahead_desc = "NO ";
		if( go_ahead == GO_AHEAD_UNDEFINED ) go_ahead_desc = "PENDING ";

		dprintf( go_ahead < 0 ? D_ALWAYS : D_FULLDEBUG,
				 "Sending %sGoAhead for %s to %s %s%s.\n",
				 go_ahead_desc,
				 ip ? ip : "(null)",
				 downloading ? "send" : "receive",
				 full_fname,
				 (go_ahead == GO_AHEAD_ALWAYS) ? " and all further files":"");

		s->encode();
		msg.Assign(ATTR_RESULT,go_ahead);  
		if( go_ahead < 0 ) {
			msg.Assign(ATTR_TRY_AGAIN,try_again);
			msg.Assign(ATTR_HOLD_REASON_CODE,hold_code);
			msg.Assign(ATTR_HOLD_REASON_SUBCODE,hold_subcode);
			if( error_desc.Length() ) {
				msg.Assign(ATTR_HOLD_REASON,error_desc.Value());
			}
		}
		if( !msg.put(*s) || !s->end_of_message() ) {
			error_desc.sprintf("Failed to send GoAhead message.");
			try_again = true;
			return false;
		}
		last_alive = time(NULL);

		if( go_ahead != GO_AHEAD_UNDEFINED ) {
			break;
		}
	}

	if( go_ahead == GO_AHEAD_ALWAYS ) {
		go_ahead_always = true;
	}

	return go_ahead > 0;
}
