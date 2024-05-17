do_cleanup(Authctxt *authctxt)
{
	static int called = 0;

	debug("do_cleanup");

	 
	if (is_child)
		return;

	 
	if (called)
		return;
	called = 1;

	if (authctxt == NULL)
		return;

#ifdef USE_PAM
	if (options.use_pam) {
		sshpam_cleanup();
		sshpam_thread_cleanup();
	}
#endif

	if (!authctxt->authenticated)
		return;

#ifdef KRB5
	if (options.kerberos_ticket_cleanup &&
	    authctxt->krb5_ctx)
		krb5_cleanup_proc(authctxt);
#endif

#ifdef GSSAPI
	if (compat20 && options.gss_cleanup_creds)
		ssh_gssapi_cleanup_creds();
#endif

	 
	auth_sock_cleanup_proc(authctxt->pw);

	 
	if (!use_privsep || mm_is_monitor())
		session_destroy_all(session_pty_cleanup2);
}
