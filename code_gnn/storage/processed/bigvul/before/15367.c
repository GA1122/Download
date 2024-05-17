SSH_PACKET_CALLBACK(ssh_packet_disconnect_callback){
  int rc;
  uint32_t code = 0;
  char *error = NULL;
  ssh_string error_s;
  (void)user;
  (void)type;

  rc = buffer_get_u32(packet, &code);
  if (rc != 0) {
    code = ntohl(code);
  }

  error_s = buffer_get_ssh_string(packet);
  if (error_s != NULL) {
    error = ssh_string_to_char(error_s);
    ssh_string_free(error_s);
  }
  SSH_LOG(SSH_LOG_PACKET, "Received SSH_MSG_DISCONNECT %d:%s",
                          code, error != NULL ? error : "no error");
  ssh_set_error(session, SSH_FATAL,
      "Received SSH_MSG_DISCONNECT: %d:%s",
      code, error != NULL ? error : "no error");
  SAFE_FREE(error);

  ssh_socket_close(session->socket);
  session->alive = 0;
  session->session_state = SSH_SESSION_STATE_ERROR;
   
  return SSH_PACKET_USED;
}
