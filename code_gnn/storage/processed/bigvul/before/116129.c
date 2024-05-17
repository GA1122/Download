bool ResourceDispatcherHostImpl::CompleteResponseStarted(
    net::URLRequest* request) {
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  scoped_refptr<ResourceResponse> response(new ResourceResponse);
  PopulateResourceResponse(request, response);

  if (request->ssl_info().cert) {
    int cert_id =
        CertStore::GetInstance()->StoreCert(request->ssl_info().cert,
                                            info->GetChildID());
    response->security_info = SerializeSecurityInfo(
        cert_id, request->ssl_info().cert_status,
        request->ssl_info().security_bits,
        request->ssl_info().connection_status);
  } else {
    DCHECK(!request->ssl_info().cert_status &&
           request->ssl_info().security_bits == -1 &&
           !request->ssl_info().connection_status);
  }

  NotifyResponseStarted(request, info->GetChildID());
  info->set_called_on_response_started(true);

  bool defer = false;
  if (!info->resource_handler()->OnResponseStarted(info->GetRequestID(),
                                                   response.get(),
                                                   &defer))
    return false;

  if (defer)
    PauseRequest(info->GetChildID(), info->GetRequestID(), true);

  return true;
}
