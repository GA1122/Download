void FetchManager::Loader::PerformHTTPFetch() {

  ResourceRequest request(fetch_request_data_->Url());
  request.SetRequestContext(fetch_request_data_->Context());
  request.SetHTTPMethod(fetch_request_data_->Method());

  switch (fetch_request_data_->Mode()) {
    case FetchRequestMode::kSameOrigin:
    case FetchRequestMode::kNoCORS:
    case FetchRequestMode::kCORS:
    case FetchRequestMode::kCORSWithForcedPreflight:
      request.SetFetchRequestMode(fetch_request_data_->Mode());
      break;
    case FetchRequestMode::kNavigate:
      request.SetFetchRequestMode(FetchRequestMode::kSameOrigin);
      break;
  }

  request.SetFetchCredentialsMode(fetch_request_data_->Credentials());
  for (const auto& header : fetch_request_data_->HeaderList()->List()) {
    DCHECK(!CORS::IsForbiddenHeaderName(header.first));

    request.AddHTTPHeaderField(AtomicString(header.first),
                               AtomicString(header.second));
  }

  if (fetch_request_data_->Method() != HTTPNames::GET &&
      fetch_request_data_->Method() != HTTPNames::HEAD) {
    if (fetch_request_data_->Buffer())
      request.SetHTTPBody(fetch_request_data_->Buffer()->DrainAsFormData());
  }
  request.SetCacheMode(fetch_request_data_->CacheMode());
  request.SetFetchRedirectMode(fetch_request_data_->Redirect());
  request.SetUseStreamOnResponse(true);
  request.SetExternalRequestStateFromRequestorAddressSpace(
      execution_context_->GetSecurityContext().AddressSpace());

  const ReferrerPolicy referrer_policy =
      fetch_request_data_->GetReferrerPolicy() == kReferrerPolicyDefault
          ? execution_context_->GetReferrerPolicy()
          : fetch_request_data_->GetReferrerPolicy();
  const String referrer_string =
      fetch_request_data_->ReferrerString() ==
              FetchRequestData::ClientReferrerString()
          ? execution_context_->OutgoingReferrer()
          : fetch_request_data_->ReferrerString();
  request.SetHTTPReferrer(SecurityPolicy::GenerateReferrer(
      referrer_policy, fetch_request_data_->Url(), referrer_string));
  request.SetSkipServiceWorker(is_isolated_world_);

  if (fetch_request_data_->Keepalive()) {
    if (!CORS::IsCORSSafelistedMethod(request.HttpMethod()) ||
        !CORS::ContainsOnlyCORSSafelistedOrForbiddenHeaders(
            request.HttpHeaderFields())) {
      PerformNetworkError(
          "Preflight request for request with keepalive "
          "specified is currently not supported");
      return;
    }
    request.SetKeepalive(true);
  }




  ResourceLoaderOptions resource_loader_options;
  resource_loader_options.data_buffering_policy = kDoNotBufferData;
  resource_loader_options.security_origin = fetch_request_data_->Origin().get();
  if (fetch_request_data_->URLLoaderFactory()) {
    network::mojom::blink::URLLoaderFactoryPtr factory_clone;
    fetch_request_data_->URLLoaderFactory()->Clone(MakeRequest(&factory_clone));
    resource_loader_options.url_loader_factory = base::MakeRefCounted<
        base::RefCountedData<network::mojom::blink::URLLoaderFactoryPtr>>(
        std::move(factory_clone));
  }

  ThreadableLoaderOptions threadable_loader_options;

  probe::willStartFetch(execution_context_, this);
  threadable_loader_ = ThreadableLoader::Create(*execution_context_, this,
                                                threadable_loader_options,
                                                resource_loader_options);
  threadable_loader_->Start(request);
}
