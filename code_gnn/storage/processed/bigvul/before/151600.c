CORSStatus ResourceLoader::DetermineCORSStatus(const ResourceResponse& response,
                                               StringBuilder& error_msg) const {
  if (response.WasFetchedViaServiceWorker()) {
    switch (response.ResponseTypeViaServiceWorker()) {
      case network::mojom::FetchResponseType::kBasic:
      case network::mojom::FetchResponseType::kCORS:
      case network::mojom::FetchResponseType::kDefault:
      case network::mojom::FetchResponseType::kError:
        return CORSStatus::kServiceWorkerSuccessful;
      case network::mojom::FetchResponseType::kOpaque:
      case network::mojom::FetchResponseType::kOpaqueRedirect:
        return CORSStatus::kServiceWorkerOpaque;
    }
    NOTREACHED();
  }

  if (resource_->GetType() == Resource::Type::kMainResource)
    return CORSStatus::kNotApplicable;

  SecurityOrigin* source_origin = resource_->Options().security_origin.get();

  if (!source_origin)
    source_origin = Context().GetSecurityOrigin();

  DCHECK(source_origin);

  if (source_origin->CanRequestNoSuborigin(response.Url()))
    return CORSStatus::kSameOrigin;

  const ResourceRequest& initial_request = resource_->GetResourceRequest();

  if (resource_->Options().cors_handling_by_resource_fetcher !=
          kEnableCORSHandlingByResourceFetcher ||
      initial_request.GetFetchRequestMode() !=
          WebURLRequest::kFetchRequestModeCORS)
    return CORSStatus::kNotApplicable;

  const ResourceResponse& response_for_access_control =
      (resource_->IsCacheValidator() && response.HttpStatusCode() == 304)
          ? resource_->GetResponse()
          : response;

  WebCORS::AccessStatus cors_status =
      WebCORS::CheckAccess(response_for_access_control.Url(),
                           response_for_access_control.HttpStatusCode(),
                           response_for_access_control.HttpHeaderFields(),
                           initial_request.GetFetchCredentialsMode(),
                           WebSecurityOrigin(source_origin));

  if (cors_status == WebCORS::AccessStatus::kAccessAllowed)
    return CORSStatus::kSuccessful;

  String resource_type = Resource::ResourceTypeToString(
      resource_->GetType(), resource_->Options().initiator_info.name);
  error_msg.Append("Access to ");
  error_msg.Append(resource_type);
  error_msg.Append(" at '");
  error_msg.Append(response.Url().GetString());
  error_msg.Append("' from origin '");
  error_msg.Append(source_origin->ToString());
  error_msg.Append("' has been blocked by CORS policy: ");
  error_msg.Append(WebCORS::AccessControlErrorString(
      cors_status, response_for_access_control.HttpStatusCode(),
      response_for_access_control.HttpHeaderFields(),
      WebSecurityOrigin(source_origin), initial_request.GetRequestContext()));

  return CORSStatus::kFailed;
}
