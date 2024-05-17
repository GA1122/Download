void ResourceDispatcherHost::OnReceivedRedirect(URLRequest* request,
                                                const GURL& new_url,
                                                bool* defer_redirect) {
  RESOURCE_LOG("OnReceivedRedirect: " << request->url().spec());
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);

  DCHECK(request->status().is_success());

  if (info->process_type() != ChildProcessInfo::PLUGIN_PROCESS &&
      !ChildProcessSecurityPolicy::GetInstance()->
          CanRequestURL(info->child_id(), new_url)) {
    LOG(INFO) << "Denied unauthorized request for " <<
        new_url.possibly_invalid_spec();

    CancelRequest(info->child_id(), info->request_id(), false);
    return;
  }

  NotifyReceivedRedirect(request, info->child_id(), new_url);

  if (HandleExternalProtocol(info->request_id(), info->child_id(),
                             info->route_id(), new_url,
                             info->resource_type(), info->resource_handler())) {
    RemovePendingRequest(info->child_id(), info->request_id());
    return;
  }

  scoped_refptr<ResourceResponse> response = new ResourceResponse;
  PopulateResourceResponse(request, info->filter_policy(), response);
  if (!info->resource_handler()->OnRequestRedirected(info->request_id(),
                                                     new_url,
                                                     response, defer_redirect))
    CancelRequest(info->child_id(), info->request_id(), false);
}
