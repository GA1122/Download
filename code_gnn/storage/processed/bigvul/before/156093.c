std::unique_ptr<HttpResponse> HandlePrefixedRequest(
    const std::string& prefix,
    const EmbeddedTestServer::HandleRequestCallback& handler,
    const HttpRequest& request) {
  if (ShouldHandle(request, prefix))
    return handler.Run(request);
  return nullptr;
}
