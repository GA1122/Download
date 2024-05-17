static void dns_resolve_recv(struct dgram_conn *dgram)
{
	struct dns_nameserver *ns, *tmpns;
	struct dns_resolvers  *resolvers;
	struct dns_resolution *res;
	struct dns_query_item *query;
	unsigned char  buf[DNS_MAX_UDP_MESSAGE + 1];
	unsigned char *bufend;
	int fd, buflen, dns_resp;
	int max_answer_records;
	unsigned short query_id;
	struct eb32_node *eb;
	struct dns_requester *req;

	fd = dgram->t.sock.fd;

	 
	if (!fd_recv_ready(fd))
		return;

	 
	if ((ns = dgram->owner) == NULL)
		return;

	resolvers = ns->resolvers;
	HA_SPIN_LOCK(DNS_LOCK, &resolvers->lock);

	 
	while (1) {
		 
		memset(buf, '\0', resolvers->accepted_payload_size + 1);
		if ((buflen = recv(fd, (char*)buf , resolvers->accepted_payload_size + 1, 0)) < 0) {
			 
			fd_cant_recv(fd);
			break;
		}

		 
		if (buflen > resolvers->accepted_payload_size) {
			ns->counters.too_big++;
			continue;
		}

		 
		bufend = buf + buflen;	 

		 
		if (buf + 2 > bufend) {
			ns->counters.invalid++;
			continue;
		}
		query_id = dns_response_get_query_id(buf);

		 
		eb = eb32_lookup(&resolvers->query_ids, query_id);
		if (eb == NULL) {
			 
			ns->counters.outdated++;
			continue;
		}

		 
		res = eb32_entry(eb, struct dns_resolution, qid);
		if (!res) {
			ns->counters.outdated++;
			continue;
		}

		 
		res->nb_responses++;

		max_answer_records = (resolvers->accepted_payload_size - DNS_HEADER_SIZE) / DNS_MIN_RECORD_SIZE;
		dns_resp = dns_validate_dns_response(buf, bufend, res, max_answer_records);

		switch (dns_resp) {
			case DNS_RESP_VALID:
				break;

			case DNS_RESP_INVALID:
			case DNS_RESP_QUERY_COUNT_ERROR:
			case DNS_RESP_WRONG_NAME:
				res->status = RSLV_STATUS_INVALID;
				ns->counters.invalid++;
				break;

			case DNS_RESP_NX_DOMAIN:
				res->status = RSLV_STATUS_NX;
				ns->counters.nx++;
				break;

			case DNS_RESP_REFUSED:
				res->status = RSLV_STATUS_REFUSED;
				ns->counters.refused++;
				break;

			case DNS_RESP_ANCOUNT_ZERO:
				res->status = RSLV_STATUS_OTHER;
				ns->counters.any_err++;
				break;

			case DNS_RESP_CNAME_ERROR:
				res->status = RSLV_STATUS_OTHER;
				ns->counters.cname_error++;
				break;

			case DNS_RESP_TRUNCATED:
				res->status = RSLV_STATUS_OTHER;
				ns->counters.truncated++;
				break;

			case DNS_RESP_NO_EXPECTED_RECORD:
			case DNS_RESP_ERROR:
			case DNS_RESP_INTERNAL:
				res->status = RSLV_STATUS_OTHER;
				ns->counters.other++;
				break;
		}

		 
		if (dns_resp != DNS_RESP_VALID && res->nb_responses < resolvers->nb_nameservers)
			continue;

		 
		if (dns_resp != DNS_RESP_VALID)  {
			if (res->prefered_query_type != res->query_type) {
				 
				if (!res->try)
					goto report_res_error;
			}
			else {
				 
				if (res->prefered_query_type == DNS_RTYPE_A) {
					res->query_type = DNS_RTYPE_AAAA;
					dns_send_query(res);
				}
				else if (res->prefered_query_type == DNS_RTYPE_AAAA) {
					res->query_type = DNS_RTYPE_A;
					dns_send_query(res);
				}
			}
			continue;
		}

		 
		query = LIST_NEXT(&res->response.query_list, struct dns_query_item *, list);
		if (query && memcmp(query->name, res->hostname_dn, res->hostname_dn_len) != 0) {
			dns_resp = DNS_RESP_WRONG_NAME;
			ns->counters.other++;
			goto report_res_error;
		}

		 
		res->status     = RSLV_STATUS_VALID;
		res->last_valid = now_ms;
		ns->counters.valid++;
		goto report_res_success;

	report_res_error:
		list_for_each_entry(req, &res->requesters, list)
			req->requester_error_cb(req, dns_resp);
		dns_reset_resolution(res);
		LIST_DEL(&res->list);
		LIST_ADDQ(&resolvers->resolutions.wait, &res->list);
		continue;

	report_res_success:
		 
		tmpns = ns;
		list_for_each_entry(req, &res->requesters, list) {
			struct server *s = objt_server(req->owner);

			if (s)
				HA_SPIN_LOCK(SERVER_LOCK, &s->lock);
			req->requester_cb(req, tmpns);
			if (s)
				HA_SPIN_UNLOCK(SERVER_LOCK, &s->lock);
			tmpns = NULL;
		}

		dns_reset_resolution(res);
		LIST_DEL(&res->list);
		LIST_ADDQ(&resolvers->resolutions.wait, &res->list);
		continue;
	}
	dns_update_resolvers_timeout(resolvers);
	HA_SPIN_UNLOCK(DNS_LOCK, &resolvers->lock);
}
