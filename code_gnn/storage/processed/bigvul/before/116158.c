void PopulateResourceResponse(net::URLRequest* request,
                              ResourceResponse* response) {
  response->status = request->status();
  response->request_time = request->request_time();
  response->response_time = request->response_time();
  response->headers = request->response_headers();
  request->GetCharset(&response->charset);
  response->content_length = request->GetExpectedContentSize();
  request->GetMimeType(&response->mime_type);
  net::HttpResponseInfo response_info = request->response_info();
  response->was_fetched_via_spdy = response_info.was_fetched_via_spdy;
  response->was_npn_negotiated = response_info.was_npn_negotiated;
  response->npn_negotiated_protocol = response_info.npn_negotiated_protocol;
  response->was_fetched_via_proxy = request->was_fetched_via_proxy();
  response->socket_address = request->GetSocketAddress();
  appcache::AppCacheInterceptor::GetExtraResponseInfo(
      request,
      &response->appcache_id,
      &response->appcache_manifest_url);
}
