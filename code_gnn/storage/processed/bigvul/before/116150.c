void ResourceDispatcherHostImpl::OnReceivedRedirect(net::URLRequest* request,
                                                    const GURL& new_url,
                                                    bool* defer_redirect) {
  VLOG(1) << "OnReceivedRedirect: " << request->url().spec();
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  DCHECK(request->status().is_success());

  if (info->process_type() != PROCESS_TYPE_PLUGIN &&
      !ChildProcessSecurityPolicyImpl::GetInstance()->
          CanRequestURL(info->GetChildID(), new_url)) {
    VLOG(1) << "Denied unauthorized request for "
            << new_url.possibly_invalid_spec();

    CancelRequestInternal(request, false);
    return;
  }

  NotifyReceivedRedirect(request, info->GetChildID(), new_url);

  if (HandleExternalProtocol(info->GetRequestID(), info->GetChildID(),
                             info->GetRouteID(), new_url,
                             info->GetResourceType(),
                             *request->context()->job_factory(),
                             info->resource_handler())) {
    RemovePendingRequest(info->GetChildID(), info->GetRequestID());
    return;
  }

  scoped_refptr<ResourceResponse> response(new ResourceResponse);
  PopulateResourceResponse(request, response);
  if (!info->resource_handler()->OnRequestRedirected(info->GetRequestID(),
                                                     new_url,
                                                     response, defer_redirect))
    CancelRequestInternal(request, false);
}
