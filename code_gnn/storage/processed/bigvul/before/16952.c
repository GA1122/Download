WebContext::BrowserContextDelegate::CreateCustomURLRequestJob(
    net::URLRequest* request,
    net::NetworkDelegate* network_delegate) {
  return new URLRequestDelegatedJob(context_getter_.get(),
                                    request,
                                    network_delegate);
}
