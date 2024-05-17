 void SendNoContentResponse(struct mg_connection* connection,
                            const struct mg_request_info* request_info,
                            void* user_data) {
  std::string response = "HTTP/1.1 204 No Content\r\n"
                         "Content-Length:0\r\n"
                         "\r\n";
  mg_write(connection, response.data(), response.length());
}
