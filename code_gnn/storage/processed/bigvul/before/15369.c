static int server_set_kex(ssh_session session) {
  struct ssh_kex_struct *server = &session->next_crypto->server_kex;
  int i, j, rc;
  const char *wanted;
  char hostkeys[64] = {0};
  enum ssh_keytypes_e keytype;
  size_t len;

  ZERO_STRUCTP(server);
  ssh_get_random(server->cookie, 16, 0);

#ifdef HAVE_ECC
  if (session->srv.ecdsa_key != NULL) {
      snprintf(hostkeys, sizeof(hostkeys),
               "%s", session->srv.ecdsa_key->type_c);
  }
#endif
  if (session->srv.dsa_key != NULL) {
      len = strlen(hostkeys);
      keytype = ssh_key_type(session->srv.dsa_key);

      snprintf(hostkeys + len, sizeof(hostkeys) - len,
               ",%s", ssh_key_type_to_char(keytype));
  }
  if (session->srv.rsa_key != NULL) {
      len = strlen(hostkeys);
      keytype = ssh_key_type(session->srv.rsa_key);

      snprintf(hostkeys + len, sizeof(hostkeys) - len,
               ",%s", ssh_key_type_to_char(keytype));
  }

  if (strlen(hostkeys) == 0) {
      return -1;
  }

  rc = ssh_options_set_algo(session,
                            SSH_HOSTKEYS,
                            hostkeys[0] == ',' ? hostkeys + 1 : hostkeys);
  if (rc < 0) {
      return -1;
  }

  for (i = 0; i < 10; i++) {
    if ((wanted = session->opts.wanted_methods[i]) == NULL) {
      wanted = ssh_kex_get_supported_method(i);
    }
    server->methods[i] = strdup(wanted);
    if (server->methods[i] == NULL) {
      for (j = 0; j < i; j++) {
        SAFE_FREE(server->methods[j]);
      }
      return -1;
    }
  }

  return 0;
}
