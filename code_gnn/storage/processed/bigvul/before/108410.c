net::HostResolver* ChromeRenderMessageFilter::GetHostResolver() {
  return request_context_->GetURLRequestContext()->host_resolver();
}
