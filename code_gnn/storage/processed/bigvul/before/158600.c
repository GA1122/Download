base::Optional<ResourceRequestBlockedReason> FrameFetchContext::CanRequest(
    Resource::Type type,
    const ResourceRequest& resource_request,
    const KURL& url,
    const ResourceLoaderOptions& options,
    SecurityViolationReportingPolicy reporting_policy,
    ResourceRequest::RedirectStatus redirect_status) const {
  if (document_ && document_->IsFreezingInProgress() &&
      !resource_request.GetKeepalive()) {
    AddErrorConsoleMessage(
        "Only fetch keepalive is allowed during onfreeze: " + url.GetString(),
        kJSSource);
    return ResourceRequestBlockedReason::kOther;
  }
  return BaseFetchContext::CanRequest(type, resource_request, url, options,
                                      reporting_policy, redirect_status);
}
