reply_handle(struct request *const req, u16 flags, u32 ttl, struct reply *reply) {
	int error;
	char addrbuf[128];
	static const int error_codes[] = {
		DNS_ERR_FORMAT, DNS_ERR_SERVERFAILED, DNS_ERR_NOTEXIST,
		DNS_ERR_NOTIMPL, DNS_ERR_REFUSED
	};

	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);

	if (flags & 0x020f || !reply || !reply->have_answer) {
		 
		if (flags & 0x0200) {
			error = DNS_ERR_TRUNCATED;
		} else if (flags & 0x000f) {
			u16 error_code = (flags & 0x000f) - 1;
			if (error_code > 4) {
				error = DNS_ERR_UNKNOWN;
			} else {
				error = error_codes[error_code];
			}
		} else if (reply && !reply->have_answer) {
			error = DNS_ERR_NODATA;
		} else {
			error = DNS_ERR_UNKNOWN;
		}

		switch (error) {
		case DNS_ERR_NOTIMPL:
		case DNS_ERR_REFUSED:
			 
			if (req->reissue_count < req->base->global_max_reissues) {
				char msg[64];
				evutil_snprintf(msg, sizeof(msg), "Bad response %d (%s)",
					 error, evdns_err_to_string(error));
				nameserver_failed(req->ns, msg);
				if (!request_reissue(req)) return;
			}
			break;
		case DNS_ERR_SERVERFAILED:
			 
			log(EVDNS_LOG_DEBUG, "Got a SERVERFAILED from nameserver"
				"at %s; will allow the request to time out.",
			    evutil_format_sockaddr_port_(
				    (struct sockaddr *)&req->ns->address,
				    addrbuf, sizeof(addrbuf)));
			 
			evdns_request_timeout_callback(0, 0, req);
			return;
		default:
			 
			if (req->handle == req->ns->probe_request) {
				 
				req->ns->probe_request = NULL;
			}

			nameserver_up(req->ns);
		}

		if (req->handle->search_state &&
		    req->request_type != TYPE_PTR) {
			 
			if (!search_try_next(req->handle)) {
				 
				 
				 
				return;
			}
		}

		 
		reply_schedule_callback(req, ttl, error, NULL);
		request_finished(req, &REQ_HEAD(req->base, req->trans_id), 1);
	} else {
		 
		reply_schedule_callback(req, ttl, 0, reply);
		if (req->handle == req->ns->probe_request)
			req->ns->probe_request = NULL;  
		nameserver_up(req->ns);
		request_finished(req, &REQ_HEAD(req->base, req->trans_id), 1);
	}
}
