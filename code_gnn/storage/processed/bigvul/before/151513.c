bool WorkerFetchContext::ShouldBlockFetchByMixedContentCheck(
    WebURLRequest::RequestContext request_context,
    WebURLRequest::FrameType frame_type,
    ResourceRequest::RedirectStatus redirect_status,
    const KURL& url,
    SecurityViolationReportingPolicy reporting_policy) const {
  return MixedContentChecker::ShouldBlockFetchOnWorker(
      global_scope_, web_context_.get(), request_context, frame_type,
      redirect_status, url, reporting_policy);
}
