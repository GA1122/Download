static int ceph_x_build_request(struct ceph_auth_client *ac,
				void *buf, void *end)
{
	struct ceph_x_info *xi = ac->private;
	int need;
	struct ceph_x_request_header *head = buf;
	int ret;
	struct ceph_x_ticket_handler *th =
		get_ticket_handler(ac, CEPH_ENTITY_TYPE_AUTH);

	if (IS_ERR(th))
		return PTR_ERR(th);

	ceph_x_validate_tickets(ac, &need);

	dout("build_request want %x have %x need %x\n",
	     ac->want_keys, xi->have_keys, need);

	if (need & CEPH_ENTITY_TYPE_AUTH) {
		struct ceph_x_authenticate *auth = (void *)(head + 1);
		void *p = auth + 1;
		struct ceph_x_challenge_blob tmp;
		char tmp_enc[40];
		u64 *u;

		if (p > end)
			return -ERANGE;

		dout(" get_auth_session_key\n");
		head->op = cpu_to_le16(CEPHX_GET_AUTH_SESSION_KEY);

		 
		get_random_bytes(&auth->client_challenge, sizeof(u64));
		tmp.client_challenge = auth->client_challenge;
		tmp.server_challenge = cpu_to_le64(xi->server_challenge);
		ret = ceph_x_encrypt(&xi->secret, &tmp, sizeof(tmp),
				     tmp_enc, sizeof(tmp_enc));
		if (ret < 0)
			return ret;

		auth->struct_v = 1;
		auth->key = 0;
		for (u = (u64 *)tmp_enc; u + 1 <= (u64 *)(tmp_enc + ret); u++)
			auth->key ^= *(__le64 *)u;
		dout(" server_challenge %llx client_challenge %llx key %llx\n",
		     xi->server_challenge, le64_to_cpu(auth->client_challenge),
		     le64_to_cpu(auth->key));

		 
		ret = ceph_x_encode_ticket(th, &p, end);
		if (ret < 0)
			return ret;

		return p - buf;
	}

	if (need) {
		void *p = head + 1;
		struct ceph_x_service_ticket_request *req;

		if (p > end)
			return -ERANGE;
		head->op = cpu_to_le16(CEPHX_GET_PRINCIPAL_SESSION_KEY);

		ret = ceph_x_build_authorizer(ac, th, &xi->auth_authorizer);
		if (ret)
			return ret;
		ceph_encode_copy(&p, xi->auth_authorizer.buf->vec.iov_base,
				 xi->auth_authorizer.buf->vec.iov_len);

		req = p;
		req->keys = cpu_to_le32(need);
		p += sizeof(*req);
		return p - buf;
	}

	return 0;
}