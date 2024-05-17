static connection_struct *switch_message(uint8 type, struct smb_request *req, int size)
{
	int flags;
	uint16 session_tag;
	connection_struct *conn = NULL;
	struct smbd_server_connection *sconn = smbd_server_conn;

	errno = 0;

	 
	if (!valid_smb_header(req->inbuf)
	    || (size < (smb_size - 4))) {
		DEBUG(2,("Non-SMB packet of length %d. Terminating server\n",
			 smb_len(req->inbuf)));
		exit_server_cleanly("Non-SMB packet");
	}

	if (smb_messages[type].fn == NULL) {
		DEBUG(0,("Unknown message type %d!\n",type));
		smb_dump("Unknown", 1, (char *)req->inbuf, size);
		reply_unknown_new(req, type);
		return NULL;
	}

	flags = smb_messages[type].flags;

	 
	session_tag = (lp_security() == SEC_SHARE)
		? UID_FIELD_INVALID : req->vuid;
	conn = req->conn;

	DEBUG(3,("switch message %s (pid %d) conn 0x%lx\n", smb_fn_name(type),
		 (int)sys_getpid(), (unsigned long)conn));

	smb_dump(smb_fn_name(type), 1, (char *)req->inbuf, size);

	 
	SSVAL(req->inbuf,smb_uid,session_tag);

	 

	if (session_tag != sconn->smb1.sessions.last_session_tag) {
		user_struct *vuser = NULL;

		sconn->smb1.sessions.last_session_tag = session_tag;
		if(session_tag != UID_FIELD_INVALID) {
			vuser = get_valid_user_struct(sconn, session_tag);
			if (vuser) {
				set_current_user_info(
					vuser->server_info->sanitized_username,
					vuser->server_info->unix_name,
					pdb_get_domain(vuser->server_info
						       ->sam_account));
			}
		}
	}

	 
	if (flags & AS_USER) {

		 
		if (!conn) {
			 
			if (type == SMBntcreateX) {
				reply_nterror(req, NT_STATUS_INVALID_HANDLE);
			} else {
				reply_nterror(req, NT_STATUS_NETWORK_NAME_DELETED);
			}
			return NULL;
		}

		if (!change_to_user(conn,session_tag)) {
			DEBUG(0, ("Error: Could not change to user. Removing "
			    "deferred open, mid=%d.\n", req->mid));
			reply_force_doserror(req, ERRSRV, ERRbaduid);
			return conn;
		}

		 

		 
		if ((flags & NEED_WRITE) && !CAN_WRITE(conn)) {
			reply_nterror(req, NT_STATUS_MEDIA_WRITE_PROTECTED);
			return conn;
		}

		 
		if (IS_IPC(conn) && !(flags & CAN_IPC)) {
			reply_nterror(req, NT_STATUS_ACCESS_DENIED);
			return conn;
		}
	} else {
		 
		change_to_root_user();
	}

	 
	if (conn) {
		if (req->encrypted) {
			conn->encrypted_tid = true;
			 
			conn->encrypt_level = Required;
		} else if (ENCRYPTION_REQUIRED(conn)) {
			if (req->cmd != SMBtrans2 && req->cmd != SMBtranss2) {
				exit_server_cleanly("encryption required "
					"on connection");
				return conn;
			}
		}

		if (!set_current_service(conn,SVAL(req->inbuf,smb_flg),
					 (flags & (AS_USER|DO_CHDIR)
					  ?True:False))) {
			reply_nterror(req, NT_STATUS_ACCESS_DENIED);
			return conn;
		}
		conn->num_smb_operations++;
	}

	 
	if ((flags & AS_GUEST)
	    && (!change_to_guest() ||
		!check_access(smbd_server_fd(), lp_hostsallow(-1),
			      lp_hostsdeny(-1)))) {
		reply_nterror(req, NT_STATUS_ACCESS_DENIED);
		return conn;
	}

	smb_messages[type].fn(req);
	return req->conn;
}
