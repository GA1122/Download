int32_t TestURLLoader::OpenTrusted(const std::string& method,
                                   const std::string& header) {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod(method);
  request.SetHeaders(header);

  return OpenTrusted(request, NULL);
}
