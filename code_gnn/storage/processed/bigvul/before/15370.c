static int ssh_server_kexdh_init(ssh_session session, ssh_buffer packet){
    ssh_string e;
    e = buffer_get_ssh_string(packet);
    if (e == NULL) {
      ssh_set_error(session, SSH_FATAL, "No e number in client request");
      return -1;
    }
    if (dh_import_e(session, e) < 0) {
      ssh_set_error(session, SSH_FATAL, "Cannot import e number");
      session->session_state=SSH_SESSION_STATE_ERROR;
    } else {
      session->dh_handshake_state=DH_STATE_INIT_SENT;
      dh_handshake_server(session);
    }
    ssh_string_free(e);
    return SSH_OK;
 }
