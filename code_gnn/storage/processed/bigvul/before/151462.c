  ResourceRequestBlockedReason CanRequestInternal(
      SecurityViolationReportingPolicy reporting_policy) {
    KURL input_url(kParsedURLString, "http://example.com/");
    ResourceRequest resource_request(input_url);
    resource_request.SetFetchCredentialsMode(
        WebURLRequest::kFetchCredentialsModeOmit);
    ResourceLoaderOptions options;
    return fetch_context->CanRequest(
        Resource::kImage, resource_request, input_url, options,
        reporting_policy, FetchParameters::kUseDefaultOriginRestrictionForType,
        ResourceRequest::RedirectStatus::kNoRedirect);
  }
