bool ResourceDispatcherHost::CompleteResponseStarted(URLRequest* request) {
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);

  scoped_refptr<ResourceResponse> response = new ResourceResponse;
  PopulateResourceResponse(request, info->filter_policy(), response);

  if (request->ssl_info().cert) {
    int cert_id =
        CertStore::GetSharedInstance()->StoreCert(request->ssl_info().cert,
                                                  info->child_id());
    response->response_head.security_info =
        SSLManager::SerializeSecurityInfo(cert_id,
                                          request->ssl_info().cert_status,
                                          request->ssl_info().security_bits);
  } else {
    DCHECK(!request->ssl_info().cert_status &&
           (request->ssl_info().security_bits == -1 ||
           request->ssl_info().security_bits == 0));
  }

  NotifyResponseStarted(request, info->child_id());
  info->set_called_on_response_started(true);
  return info->resource_handler()->OnResponseStarted(info->request_id(),
                                                     response.get());
}
