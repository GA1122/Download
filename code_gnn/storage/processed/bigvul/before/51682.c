void doi_log_cert_thinking(u_int16_t auth,
				enum ike_cert_type certtype,
				enum certpolicy policy,
				bool gotcertrequest,
				bool send_cert,
				bool send_chain)
{
	DBG(DBG_CONTROL,
		DBG_log("thinking about whether to send my certificate:"));

	DBG(DBG_CONTROL, {
		struct esb_buf esb;

		DBG_log("  I have RSA key: %s cert.type: %s ",
			enum_showb(&oakley_auth_names, auth, &esb),
			enum_show(&ike_cert_type_names, certtype));
	});

	DBG(DBG_CONTROL,
		DBG_log("  sendcert: %s and I did%s get a certificate request ",
			enum_show(&certpolicy_type_names, policy),
			gotcertrequest ? "" : " not"));

	DBG(DBG_CONTROL,
		DBG_log("  so %ssend cert.", send_cert ? "" : "do not "));

	if (!send_cert) {
		if (auth == OAKLEY_PRESHARED_KEY) {
			DBG(DBG_CONTROL,
				DBG_log("I did not send a certificate "
					"because digital signatures are not "
					"being used. (PSK)"));
		} else if (certtype == CERT_NONE) {
			DBG(DBG_CONTROL,
				DBG_log("I did not send a certificate because "
					"I do not have one."));
		} else if (policy == cert_sendifasked) {
			DBG(DBG_CONTROL,
				DBG_log("I did not send my certificate "
					"because I was not asked to."));
		}
		 
	}
	if (send_chain)
		DBG(DBG_CONTROL, DBG_log("Sending one or more authcerts"));
}