mm_answer_gss_setup_ctx(int sock, Buffer *m)
{
	gss_OID_desc goid;
	OM_uint32 major;
	u_int len;

	if (!options.gss_authentication)
		fatal("%s: GSSAPI authentication not enabled", __func__);

	goid.elements = buffer_get_string(m, &len);
	goid.length = len;

	major = ssh_gssapi_server_ctx(&gsscontext, &goid);

	free(goid.elements);

	buffer_clear(m);
	buffer_put_int(m, major);

	mm_request_send(sock, MONITOR_ANS_GSSSETUP, m);

	 
	monitor_permit(mon_dispatch, MONITOR_REQ_GSSSTEP, 1);

	return (0);
}