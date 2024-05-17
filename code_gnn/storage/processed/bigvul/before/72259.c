send_pubkey_test(Authctxt *authctxt, Identity *id)
{
	u_char *blob;
	u_int bloblen, have_sig = 0;

	debug3("send_pubkey_test");

	if (key_to_blob(id->key, &blob, &bloblen) == 0) {
		 
		debug3("send_pubkey_test: cannot handle key");
		return 0;
	}
	 
	dispatch_set(SSH2_MSG_USERAUTH_PK_OK, &input_userauth_pk_ok);

	packet_start(SSH2_MSG_USERAUTH_REQUEST);
	packet_put_cstring(authctxt->server_user);
	packet_put_cstring(authctxt->service);
	packet_put_cstring(authctxt->method->name);
	packet_put_char(have_sig);
	if (!(datafellows & SSH_BUG_PKAUTH))
		packet_put_cstring(identity_sign_encode(id));
	packet_put_string(blob, bloblen);
	free(blob);
	packet_send();
	return 1;
}
