void SendForbidden(struct mg_connection* connection,
                   const struct mg_request_info* request_info,
                   void* user_data) {
  mg_printf(connection, "HTTP/1.1 403 Forbidden\r\n\r\n");
}
