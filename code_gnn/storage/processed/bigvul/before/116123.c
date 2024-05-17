bool ResourceDispatcherHostImpl::CancelRequestInternal(net::URLRequest* request,
                                                       bool from_renderer) {
  VLOG(1) << "CancelRequest: " << request->url().spec();

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  if (!from_renderer || !info->is_download()) {
    if (info->login_delegate()) {
      info->login_delegate()->OnRequestCancelled();
      info->set_login_delegate(NULL);
    }
    if (info->ssl_client_auth_handler()) {
      info->ssl_client_auth_handler()->OnRequestCancelled();
      info->set_ssl_client_auth_handler(NULL);
    }
    request->Cancel();
    DCHECK(IsValidRequest(request));
    return true;
  }

  return false;
}
