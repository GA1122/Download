cifs_demultiplex_thread(struct TCP_Server_Info *server)
{
	int length;
	unsigned int pdu_length, total_read;
	struct smb_hdr *smb_buffer = NULL;
	struct smb_hdr *bigbuf = NULL;
	struct smb_hdr *smallbuf = NULL;
	struct msghdr smb_msg;
	struct kvec iov;
	struct socket *csocket = server->ssocket;
	struct list_head *tmp, *tmp2;
	struct task_struct *task_to_wake = NULL;
	struct mid_q_entry *mid_entry;
	char temp;
	bool isLargeBuf = false;
	bool isMultiRsp;
	int reconnect;

	current->flags |= PF_MEMALLOC;
	cFYI(1, "Demultiplex PID: %d", task_pid_nr(current));

	length = atomic_inc_return(&tcpSesAllocCount);
	if (length > 1)
		mempool_resize(cifs_req_poolp, length + cifs_min_rcv,
				GFP_KERNEL);

	set_freezable();
	while (server->tcpStatus != CifsExiting) {
		if (try_to_freeze())
			continue;
		if (bigbuf == NULL) {
			bigbuf = cifs_buf_get();
			if (!bigbuf) {
				cERROR(1, "No memory for large SMB response");
				msleep(3000);
				 
				continue;
			}
		} else if (isLargeBuf) {
			 
			memset(bigbuf, 0, sizeof(struct smb_hdr));
		}

		if (smallbuf == NULL) {
			smallbuf = cifs_small_buf_get();
			if (!smallbuf) {
				cERROR(1, "No memory for SMB response");
				msleep(1000);
				 
				continue;
			}
			 
		} else  
			memset(smallbuf, 0, sizeof(struct smb_hdr));

		isLargeBuf = false;
		isMultiRsp = false;
		smb_buffer = smallbuf;
		iov.iov_base = smb_buffer;
		iov.iov_len = 4;
		smb_msg.msg_control = NULL;
		smb_msg.msg_controllen = 0;
		pdu_length = 4;  

incomplete_rcv:
		if (echo_retries > 0 &&
		    time_after(jiffies, server->lstrp +
					(echo_retries * SMB_ECHO_INTERVAL))) {
			cERROR(1, "Server %s has not responded in %d seconds. "
				  "Reconnecting...", server->hostname,
				  (echo_retries * SMB_ECHO_INTERVAL / HZ));
			cifs_reconnect(server);
			csocket = server->ssocket;
			wake_up(&server->response_q);
			continue;
		}

		length =
		    kernel_recvmsg(csocket, &smb_msg,
				&iov, 1, pdu_length, 0  );

		if (server->tcpStatus == CifsExiting) {
			break;
		} else if (server->tcpStatus == CifsNeedReconnect) {
			cFYI(1, "Reconnect after server stopped responding");
			cifs_reconnect(server);
			cFYI(1, "call to reconnect done");
			csocket = server->ssocket;
			continue;
		} else if (length == -ERESTARTSYS ||
			   length == -EAGAIN ||
			   length == -EINTR) {
			msleep(1);  
			if (pdu_length < 4) {
				iov.iov_base = (4 - pdu_length) +
							(char *)smb_buffer;
				iov.iov_len = pdu_length;
				smb_msg.msg_control = NULL;
				smb_msg.msg_controllen = 0;
				goto incomplete_rcv;
			} else
				continue;
		} else if (length <= 0) {
			cFYI(1, "Reconnect after unexpected peek error %d",
				length);
			cifs_reconnect(server);
			csocket = server->ssocket;
			wake_up(&server->response_q);
			continue;
		} else if (length < pdu_length) {
			cFYI(1, "requested %d bytes but only got %d bytes",
				  pdu_length, length);
			pdu_length -= length;
			msleep(1);
			goto incomplete_rcv;
		}

		 
		 

		 
		temp = *((char *) smb_buffer);

		 
		pdu_length = be32_to_cpu((__force __be32)smb_buffer->smb_buf_length);
		smb_buffer->smb_buf_length = pdu_length;

		cFYI(1, "rfc1002 length 0x%x", pdu_length+4);

		if (temp == (char) RFC1002_SESSION_KEEP_ALIVE) {
			continue;
		} else if (temp == (char)RFC1002_POSITIVE_SESSION_RESPONSE) {
			cFYI(1, "Good RFC 1002 session rsp");
			continue;
		} else if (temp == (char)RFC1002_NEGATIVE_SESSION_RESPONSE) {
			 
			cFYI(1, "Negative RFC1002 Session Response Error 0x%x)",
				pdu_length);
			 
			msleep(1000);
			 
			cifs_set_port((struct sockaddr *)
					&server->dstaddr, CIFS_PORT);
			cifs_reconnect(server);
			csocket = server->ssocket;
			wake_up(&server->response_q);
			continue;
		} else if (temp != (char) 0) {
			cERROR(1, "Unknown RFC 1002 frame");
			cifs_dump_mem(" Received Data: ", (char *)smb_buffer,
				      length);
			cifs_reconnect(server);
			csocket = server->ssocket;
			continue;
		}

		 
		if ((pdu_length > CIFSMaxBufSize + MAX_CIFS_HDR_SIZE - 4) ||
			    (pdu_length < sizeof(struct smb_hdr) - 1 - 4)) {
			cERROR(1, "Invalid size SMB length %d pdu_length %d",
					length, pdu_length+4);
			cifs_reconnect(server);
			csocket = server->ssocket;
			wake_up(&server->response_q);
			continue;
		}

		 
		reconnect = 0;

		if (pdu_length > MAX_CIFS_SMALL_BUFFER_SIZE - 4) {
			isLargeBuf = true;
			memcpy(bigbuf, smallbuf, 4);
			smb_buffer = bigbuf;
		}
		length = 0;
		iov.iov_base = 4 + (char *)smb_buffer;
		iov.iov_len = pdu_length;
		for (total_read = 0; total_read < pdu_length;
		     total_read += length) {
			length = kernel_recvmsg(csocket, &smb_msg, &iov, 1,
						pdu_length - total_read, 0);
			if (server->tcpStatus == CifsExiting) {
				 
				reconnect = 2;
				break;
			} else if (server->tcpStatus == CifsNeedReconnect) {
				cifs_reconnect(server);
				csocket = server->ssocket;
				 
				 
				reconnect = 1;
				break;
			} else if (length == -ERESTARTSYS ||
				   length == -EAGAIN ||
				   length == -EINTR) {
				msleep(1);  
				length = 0;
				continue;
			} else if (length <= 0) {
				cERROR(1, "Received no data, expecting %d",
					      pdu_length - total_read);
				cifs_reconnect(server);
				csocket = server->ssocket;
				reconnect = 1;
				break;
			}
		}
		if (reconnect == 2)
			break;
		else if (reconnect == 1)
			continue;

		total_read += 4;  

		dump_smb(smb_buffer, total_read);

		 
		length = checkSMB(smb_buffer, smb_buffer->Mid, total_read);
		if (length != 0)
			cifs_dump_mem("Bad SMB: ", smb_buffer,
					min_t(unsigned int, total_read, 48));

		mid_entry = NULL;
		server->lstrp = jiffies;

		spin_lock(&GlobalMid_Lock);
		list_for_each_safe(tmp, tmp2, &server->pending_mid_q) {
			mid_entry = list_entry(tmp, struct mid_q_entry, qhead);

			if ((mid_entry->mid == smb_buffer->Mid) &&
			    (mid_entry->midState == MID_REQUEST_SUBMITTED) &&
			    (mid_entry->command == smb_buffer->Command)) {
				if (length == 0 &&
				   check2ndT2(smb_buffer, server->maxBuf) > 0) {
					 
					isMultiRsp = true;
					if (mid_entry->resp_buf) {
						 
						if (coalesce_t2(smb_buffer,
							mid_entry->resp_buf)) {
							mid_entry->multiRsp =
								 true;
							break;
						} else {
							 
							mid_entry->multiEnd =
								 true;
							goto multi_t2_fnd;
						}
					} else {
						if (!isLargeBuf) {
							cERROR(1, "1st trans2 resp needs bigbuf");
					 
						} else {
							 
							mid_entry->resp_buf =
								 smb_buffer;
							mid_entry->largeBuf =
								 true;
							bigbuf = NULL;
						}
					}
					break;
				}
				mid_entry->resp_buf = smb_buffer;
				mid_entry->largeBuf = isLargeBuf;
multi_t2_fnd:
				if (length == 0)
					mid_entry->midState =
							MID_RESPONSE_RECEIVED;
				else
					mid_entry->midState =
							MID_RESPONSE_MALFORMED;
#ifdef CONFIG_CIFS_STATS2
				mid_entry->when_received = jiffies;
#endif
				list_del_init(&mid_entry->qhead);
				mid_entry->callback(mid_entry);
				break;
			}
			mid_entry = NULL;
		}
		spin_unlock(&GlobalMid_Lock);

		if (mid_entry != NULL) {
			 
			if (!isMultiRsp) {
				 
				if (isLargeBuf)
					bigbuf = NULL;
				else
					smallbuf = NULL;
			}
		} else if (length != 0) {
			 
			continue;
		} else if (!is_valid_oplock_break(smb_buffer, server) &&
			   !isMultiRsp) {
			cERROR(1, "No task to wake, unknown frame received! "
				   "NumMids %d", atomic_read(&midCount));
			cifs_dump_mem("Received Data is: ", (char *)smb_buffer,
				      sizeof(struct smb_hdr));
#ifdef CONFIG_CIFS_DEBUG2
			cifs_dump_detail(smb_buffer);
			cifs_dump_mids(server);
#endif  

		}
	}  

	 
	spin_lock(&cifs_tcp_ses_lock);
	list_del_init(&server->tcp_ses_list);
	spin_unlock(&cifs_tcp_ses_lock);

	spin_lock(&GlobalMid_Lock);
	server->tcpStatus = CifsExiting;
	spin_unlock(&GlobalMid_Lock);
	wake_up_all(&server->response_q);

	 
	 
	spin_lock(&GlobalMid_Lock);
	if (atomic_read(&server->inFlight) >= cifs_max_pending)
		atomic_set(&server->inFlight, cifs_max_pending - 1);
	 
	spin_unlock(&GlobalMid_Lock);
	 
	wake_up_all(&server->request_q);
	 
	msleep(125);

	if (server->ssocket) {
		sock_release(csocket);
		server->ssocket = NULL;
	}
	 
	cifs_buf_release(bigbuf);
	if (smallbuf)  
		cifs_small_buf_release(smallbuf);

	if (!list_empty(&server->pending_mid_q)) {
		spin_lock(&GlobalMid_Lock);
		list_for_each_safe(tmp, tmp2, &server->pending_mid_q) {
			mid_entry = list_entry(tmp, struct mid_q_entry, qhead);
			cFYI(1, "Clearing Mid 0x%x - issuing callback",
					 mid_entry->mid);
			list_del_init(&mid_entry->qhead);
			mid_entry->callback(mid_entry);
		}
		spin_unlock(&GlobalMid_Lock);
		 
		msleep(125);
	}

	if (!list_empty(&server->pending_mid_q)) {
		 
		 
		cFYI(1, "Wait for exit from demultiplex thread");
		msleep(46000);
		 
	}

	kfree(server->hostname);
	task_to_wake = xchg(&server->tsk, NULL);
	kfree(server);

	length = atomic_dec_return(&tcpSesAllocCount);
	if (length  > 0)
		mempool_resize(cifs_req_poolp, length + cifs_min_rcv,
				GFP_KERNEL);

	 
	if (!task_to_wake) {
		set_current_state(TASK_INTERRUPTIBLE);
		while (!signal_pending(current)) {
			schedule();
			set_current_state(TASK_INTERRUPTIBLE);
		}
		set_current_state(TASK_RUNNING);
	}

	module_put_and_exit(0);
}
