FileTransfer::ExitDoUpload(filesize_t *total_bytes, ReliSock *s, priv_state saved_priv, bool socket_default_crypto, bool upload_success, bool do_upload_ack, bool do_download_ack, bool try_again, int hold_code, int hold_subcode, char const *upload_error_desc,int DoUpload_exit_line)
{
	int rc = upload_success ? 0 : -1;
	bool download_success = false;
	MyString error_buf;
	MyString download_error_buf;
	char const *error_desc = NULL;

	dprintf(D_FULLDEBUG,"DoUpload: exiting at %d\n",DoUpload_exit_line);

	if( saved_priv != PRIV_UNKNOWN ) {
		_set_priv(saved_priv,__FILE__,DoUpload_exit_line,1);
	}

#ifdef WIN32
	bytesSent += (float)(signed __int64)*total_bytes;
#else 
	bytesSent += *total_bytes;
#endif

	if(do_upload_ack) {
		if(!PeerDoesTransferAck && !upload_success) {
		}
		else {
			s->snd_int(0,TRUE);

			MyString error_desc_to_send;
			if(!upload_success) {
				error_desc_to_send.sprintf("%s at %s failed to send file(s) to %s",
										   get_mySubSystem()->getName(),
										   s->my_ip_str(),
										   s->get_sinful_peer());
				if(upload_error_desc) {
					error_desc_to_send.sprintf_cat(": %s",upload_error_desc);
				}
			}
			SendTransferAck(s,upload_success,try_again,hold_code,hold_subcode,
			                error_desc_to_send.Value());
		}
	}

	if(do_download_ack) {
		GetTransferAck(s,download_success,try_again,hold_code,hold_subcode,
		               download_error_buf);
		if(!download_success) {
			rc = -1;
		}
	}

	if(rc != 0) {
		char const *receiver_ip_str = s->get_sinful_peer();
		if(!receiver_ip_str) {
			receiver_ip_str = "disconnected socket";
		}

		error_buf.sprintf("%s at %s failed to send file(s) to %s",
						  get_mySubSystem()->getName(),
						  s->my_ip_str(),receiver_ip_str);
		if(upload_error_desc) {
			error_buf.sprintf_cat(": %s",upload_error_desc);
		}

		if(!download_error_buf.IsEmpty()) {
			error_buf.sprintf_cat("; %s",download_error_buf.Value());
		}

		error_desc = error_buf.Value();
		if(!error_desc) {
			error_desc = "";
		}

		if(try_again) {
			dprintf(D_ALWAYS,"DoUpload: %s\n",error_desc);
		}
		else {
			dprintf(D_ALWAYS,"DoUpload: (Condor error code %d, subcode %d) %s\n",hold_code,hold_subcode,error_desc);
		}
	}

	s->set_crypto_mode(socket_default_crypto);

	Info.success = rc == 0;
	Info.try_again = try_again;
	Info.hold_code = hold_code;
	Info.hold_subcode = hold_subcode;
	Info.error_desc = error_desc;

	return rc;
}
