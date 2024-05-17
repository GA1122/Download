int32_t TestURLLoader::OpenUntrusted(const pp::URLRequestInfo& request,
                                     std::string* response_body) {
  return Open(request, false, response_body);
}
