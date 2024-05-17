void SendResponse(struct mg_connection* const connection,
                  const std::string& request_method,
                  const Response& response) {
  HttpResponse http_response;
  PrepareHttpResponse(response, &http_response);

  std::string message_header = base::StringPrintf("HTTP/1.1 %d %s\r\n",
      http_response.status(), http_response.GetReasonPhrase().c_str());

  typedef HttpResponse::HeaderMap::const_iterator HeaderIter;
  for (HeaderIter header = http_response.headers()->begin();
       header != http_response.headers()->end();
       ++header) {
    message_header.append(base::StringPrintf("%s:%s\r\n",
        header->first.c_str(), header->second.c_str()));
  }
  message_header.append("\r\n");

  mg_write(connection, message_header.data(), message_header.length());
  if (!ForbidsMessageBody(request_method, http_response))
    mg_write(connection, http_response.data(), http_response.length());
}
