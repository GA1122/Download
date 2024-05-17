void UrlFetcher::Core::SetRequestContext(
    net::URLRequestContextGetter* request_context_getter) {
  DCHECK(!request_context_getter_);
  request_context_getter_ = request_context_getter;
}
