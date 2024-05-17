RespondWithContentTypeHandler(const net::test_server::HttpRequest& request) {
  std::unique_ptr<net::test_server::BasicHttpResponse> response(
      new net::test_server::BasicHttpResponse());
  response->set_content_type(request.relative_url.substr(1));
  response->set_code(net::HTTP_OK);
  response->set_content("ooogaboogaboogabooga");
  return std::move(response);
}
