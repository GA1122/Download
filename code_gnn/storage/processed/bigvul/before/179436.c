 static int ceph_x_verify_authorizer_reply(struct ceph_auth_client *ac,
 					  struct ceph_authorizer *a, size_t len)
 {
 	struct ceph_x_authorizer *au = (void *)a;
  	struct ceph_x_ticket_handler *th;
  	int ret = 0;
  	struct ceph_x_authorize_reply reply;
// 	void *preply = &reply;
  	void *p = au->reply_buf;
  	void *end = p + sizeof(au->reply_buf);
  
  	th = get_ticket_handler(ac, au->service);
  	if (IS_ERR(th))
  		return PTR_ERR(th);
	ret = ceph_x_decrypt(&th->session_key, &p, end, &reply, sizeof(reply));
// 	ret = ceph_x_decrypt(&th->session_key, &p, end, &preply, sizeof(reply));
  	if (ret < 0)
  		return ret;
  	if (ret != sizeof(reply))
 		return -EPERM;
 
 	if (au->nonce + 1 != le64_to_cpu(reply.nonce_plus_one))
 		ret = -EPERM;
 	else
 		ret = 0;
 	dout("verify_authorizer_reply nonce %llx got %llx ret %d\n",
 	     au->nonce, le64_to_cpu(reply.nonce_plus_one), ret);
 	return ret;
 }