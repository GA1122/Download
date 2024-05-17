BuildObjectForResourceRequest(const ResourceRequest& request) {
  std::unique_ptr<protocol::Network::Request> request_object =
      protocol::Network::Request::create()
          .setUrl(UrlWithoutFragment(request.Url()).GetString())
          .setMethod(request.HttpMethod())
          .setHeaders(BuildObjectForHeaders(request.HttpHeaderFields()))
          .setInitialPriority(ResourcePriorityJSON(request.Priority()))
          .setReferrerPolicy(GetReferrerPolicy(request.GetReferrerPolicy()))
          .build();
  if (request.HttpBody() && !request.HttpBody()->IsEmpty()) {
    Vector<char> bytes;
    request.HttpBody()->Flatten(bytes);
    request_object->setPostData(
        String::FromUTF8WithLatin1Fallback(bytes.data(), bytes.size()));
  }
  return request_object;
}
