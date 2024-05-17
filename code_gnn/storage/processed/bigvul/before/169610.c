int32_t TestURLLoader::OpenTrusted(const pp::URLRequestInfo& request,
                                   std::string* response_body) {
  return Open(request, true, response_body);
}
