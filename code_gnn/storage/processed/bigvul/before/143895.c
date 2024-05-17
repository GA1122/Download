net::URLRequestContextGetter* WebRunnerBrowserContext::CreateRequestContext(
    content::ProtocolHandlerMap* protocol_handlers,
    content::URLRequestInterceptorScopedVector request_interceptors) {
  DCHECK(!url_request_getter_);
  url_request_getter_ = new WebRunnerURLRequestContextGetter(
      content::BrowserThread::GetTaskRunnerForThread(
          content::BrowserThread::IO),
      net_log_.get(), std::move(*protocol_handlers),
      std::move(request_interceptors));
  resource_context_->set_url_request_context_getter(url_request_getter_);
  return url_request_getter_.get();
}
