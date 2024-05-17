smtp_alert(smtp_msg_t msg_type, void* data, const char *subject, const char *body)
{
	smtp_t *smtp;
#ifdef _WITH_VRRP_
	vrrp_t *vrrp;
	vrrp_sgroup_t *vgroup;
#endif
#ifdef _WITH_LVS_
	checker_t *checker;
	virtual_server_t *vs;
	smtp_rs *rs_info;
#endif

	 
	if (LIST_ISEMPTY(global_data->email) || !global_data->smtp_server.ss_family)
		return;

	 
	smtp = (smtp_t *) MALLOC(sizeof(smtp_t));
	smtp->subject = (char *) MALLOC(MAX_HEADERS_LENGTH);
	smtp->body = (char *) MALLOC(MAX_BODY_LENGTH);
	smtp->buffer = (char *) MALLOC(SMTP_BUFFER_MAX);
	smtp->email_to = (char *) MALLOC(SMTP_BUFFER_MAX);

	 
#ifdef _WITH_LVS_
	if (msg_type == SMTP_MSG_RS) {
		checker = (checker_t *)data;
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] Realserver %s of virtual server %s - %s",
					global_data->router_id,
					FMT_RS(checker->rs, checker->vs),
					FMT_VS(checker->vs),
					checker->rs->alive ? "UP" : "DOWN");
	}
	else if (msg_type == SMTP_MSG_VS) {
		vs = (virtual_server_t *)data;
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] Virtualserver %s - %s",
					global_data->router_id,
					FMT_VS(vs),
					subject);
	}
	else if (msg_type == SMTP_MSG_RS_SHUT) {
		rs_info = (smtp_rs *)data;
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] Realserver %s of virtual server %s - %s",
					global_data->router_id,
					FMT_RS(rs_info->rs, rs_info->vs),
					FMT_VS(rs_info->vs),
					subject);
	}
	else
#endif
#ifdef _WITH_VRRP_
	if (msg_type == SMTP_MSG_VRRP) {
		vrrp = (vrrp_t *)data;
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] VRRP Instance %s - %s",
					global_data->router_id,
					vrrp->iname,
					subject);
	} else if (msg_type == SMTP_MSG_VGROUP) {
		vgroup = (vrrp_sgroup_t *)data;
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] VRRP Group %s - %s",
					global_data->router_id,
					vgroup->gname,
					subject);
	}
	else
#endif
	if (global_data->router_id)
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "[%s] %s"
				      , global_data->router_id
				      , subject);
	else
		snprintf(smtp->subject, MAX_HEADERS_LENGTH, "%s", subject);

	strncpy(smtp->body, body, MAX_BODY_LENGTH - 1);
	smtp->body[MAX_BODY_LENGTH - 1]= '\0';

	build_to_header_rcpt_addrs(smtp);

#ifdef _SMTP_ALERT_DEBUG_
	if (do_smtp_alert_debug)
		smtp_log_to_file(smtp);
	else
#endif
	smtp_connect(smtp);
}