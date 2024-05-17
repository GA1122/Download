void PopulateResourceResponse(URLRequest* request,
                              FilterPolicy::Type filter_policy,
                              ResourceResponse* response) {
  response->response_head.status = request->status();
  response->response_head.request_time = request->request_time();
  response->response_head.response_time = request->response_time();
  response->response_head.headers = request->response_headers();
  request->GetCharset(&response->response_head.charset);
  response->response_head.filter_policy = filter_policy;
  response->response_head.content_length = request->GetExpectedContentSize();
  request->GetMimeType(&response->response_head.mime_type);
  response->response_head.was_fetched_via_spdy =
      request->was_fetched_via_spdy();
  appcache::AppCacheInterceptor::GetExtraResponseInfo(
      request,
      &response->response_head.appcache_id,
      &response->response_head.appcache_manifest_url);
}
