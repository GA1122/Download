resume_copy_required_values (gnutls_session_t session)
{
   
  memcpy (session->internals.resumed_security_parameters.
	  server_random,
	  session->security_parameters.server_random, TLS_RANDOM_SIZE);
  memcpy (session->internals.resumed_security_parameters.
	  client_random,
	  session->security_parameters.client_random, TLS_RANDOM_SIZE);

   
  memcpy (session->security_parameters.current_cipher_suite.
	  suite,
	  session->internals.resumed_security_parameters.
	  current_cipher_suite.suite, 2);

  session->internals.compression_method =
    session->internals.resumed_security_parameters.read_compression_algorithm;
   

  session->security_parameters.entity =
    session->internals.resumed_security_parameters.entity;

  _gnutls_set_current_version (session,
			       session->internals.
			       resumed_security_parameters.version);

  session->security_parameters.cert_type =
    session->internals.resumed_security_parameters.cert_type;

  memcpy (session->security_parameters.session_id,
	  session->internals.resumed_security_parameters.
	  session_id, sizeof (session->security_parameters.session_id));
  session->security_parameters.session_id_size =
    session->internals.resumed_security_parameters.session_id_size;
}