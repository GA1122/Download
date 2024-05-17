bool FrameFetchContext::ShouldBlockFetchByMixedContentCheck(
    WebURLRequest::RequestContext request_context,
    network::mojom::RequestContextFrameType frame_type,
    ResourceRequest::RedirectStatus redirect_status,
    const KURL& url,
    SecurityViolationReportingPolicy reporting_policy) const {
  if (IsDetached()) {
    return false;
  }
  return MixedContentChecker::ShouldBlockFetch(GetFrame(), request_context,
                                               frame_type, redirect_status, url,
                                               reporting_policy);
}
