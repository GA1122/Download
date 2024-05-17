void ResourceLoader::DidReceiveResponse(
    const WebURLResponse& web_url_response,
    std::unique_ptr<WebDataConsumerHandle> handle) {
  DCHECK(!web_url_response.IsNull());

  Resource::Type resource_type = resource_->GetType();

  const ResourceRequest& initial_request = resource_->GetResourceRequest();
  WebURLRequest::RequestContext request_context =
      initial_request.GetRequestContext();
  WebURLRequest::FetchRequestMode fetch_request_mode =
      initial_request.GetFetchRequestMode();

  const ResourceLoaderOptions& options = resource_->Options();

  const ResourceResponse& response = web_url_response.ToResourceResponse();

  StringBuilder cors_error_msg;
  resource_->SetCORSStatus(DetermineCORSStatus(response, cors_error_msg));

  if (response.WasFetchedViaServiceWorker()) {
    if (options.cors_handling_by_resource_fetcher ==
            kEnableCORSHandlingByResourceFetcher &&
        fetch_request_mode == WebURLRequest::kFetchRequestModeCORS &&
        response.WasFallbackRequiredByServiceWorker()) {
      ResourceRequest last_request = resource_->LastResourceRequest();
      DCHECK_EQ(last_request.GetServiceWorkerMode(),
                WebURLRequest::ServiceWorkerMode::kAll);
      if (!Context().ShouldLoadNewResource(resource_type)) {
        HandleError(ResourceError::CancelledError(response.Url()));
        return;
      }
      last_request.SetServiceWorkerMode(
          WebURLRequest::ServiceWorkerMode::kForeign);
      Restart(last_request);
      return;
    }

    const KURL& original_url = response.OriginalURLViaServiceWorker();
    if (!original_url.IsEmpty()) {
      Context().CheckCSPForRequest(
          request_context, original_url, options,
          SecurityViolationReportingPolicy::kReport,
          ResourceRequest::RedirectStatus::kFollowedRedirect);

      ResourceRequestBlockedReason blocked_reason = Context().CanRequest(
          resource_type, initial_request, original_url, options,
          SecurityViolationReportingPolicy::kReport,
          FetchParameters::kUseDefaultOriginRestrictionForType,
          ResourceRequest::RedirectStatus::kFollowedRedirect);
      if (blocked_reason != ResourceRequestBlockedReason::kNone) {
        HandleError(ResourceError::CancelledDueToAccessCheckError(
            original_url, blocked_reason));
        return;
      }
    }
  } else if (options.cors_handling_by_resource_fetcher ==
                 kEnableCORSHandlingByResourceFetcher &&
             fetch_request_mode == WebURLRequest::kFetchRequestModeCORS) {
    if (!resource_->IsSameOriginOrCORSSuccessful()) {
      if (!resource_->IsUnusedPreload())
        Context().AddErrorConsoleMessage(cors_error_msg.ToString(),
                                         FetchContext::kJSSource);

      HandleError(ResourceError::CancelledDueToAccessCheckError(
          response.Url(), ResourceRequestBlockedReason::kOther));
      return;
    }
  }

  Context().DispatchDidReceiveResponse(
      resource_->Identifier(), response, initial_request.GetFrameType(),
      request_context, resource_,
      FetchContext::ResourceResponseType::kNotFromMemoryCache);

  resource_->ResponseReceived(response, std::move(handle));
  if (!resource_->Loader())
    return;

  if (response.HttpStatusCode() >= 400 &&
      !resource_->ShouldIgnoreHTTPStatusCodeErrors())
    HandleError(ResourceError::CancelledError(response.Url()));
}
