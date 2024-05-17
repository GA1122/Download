 smb_send_rqst(struct TCP_Server_Info *server, struct smb_rqst *rqst)
 {
 	int rc;
 	struct kvec *iov = rqst->rq_iov;
 	int n_vec = rqst->rq_nvec;
 	unsigned int smb_buf_length = get_rfc1002_length(iov[0].iov_base);
 	unsigned int i;
 	size_t total_len = 0, sent;
  	struct socket *ssocket = server->ssocket;
  	int val = 1;
  
// 	if (ssocket == NULL)
// 		return -ENOTSOCK;
// 
  	cFYI(1, "Sending smb: smb_len=%u", smb_buf_length);
  	dump_smb(iov[0].iov_base, iov[0].iov_len);
  
 	 
 	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
 				(char *)&val, sizeof(val));
 
 	rc = smb_send_kvec(server, iov, n_vec, &sent);
 	if (rc < 0)
 		goto uncork;
 
 	total_len += sent;
 
 	 
 	for (i = 0; i < rqst->rq_npages; i++) {
 		struct kvec p_iov;
 
 		cifs_rqst_page_to_kvec(rqst, i, &p_iov);
 		rc = smb_send_kvec(server, &p_iov, 1, &sent);
 		kunmap(rqst->rq_pages[i]);
 		if (rc < 0)
 			break;
 
 		total_len += sent;
 	}
 
 uncork:
 	 
 	val = 0;
 	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
 				(char *)&val, sizeof(val));
 
 	if ((total_len > 0) && (total_len != smb_buf_length + 4)) {
 		cFYI(1, "partial send (wanted=%u sent=%zu): terminating "
 			"session", smb_buf_length + 4, total_len);
 		 
 		server->tcpStatus = CifsNeedReconnect;
 	}
 
 	if (rc < 0 && rc != -EINTR)
 		cERROR(1, "Error %d sending data on socket to server", rc);
 	else
 		rc = 0;
 
 	return rc;
 }