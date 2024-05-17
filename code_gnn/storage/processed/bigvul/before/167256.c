void FetchManager::Loader::DidReceiveResponse(
    unsigned long,
    const ResourceResponse& response,
    std::unique_ptr<WebDataConsumerHandle> handle) {
  DCHECK(handle);
  DCHECK(response.Url() == url_list_.back());
  ScriptState* script_state = resolver_->GetScriptState();
  ScriptState::Scope scope(script_state);

  response_http_status_code_ = response.HttpStatusCode();
  FetchRequestData::Tainting tainting = fetch_request_data_->ResponseTainting();

  if (response.Url().ProtocolIsData()) {
    if (fetch_request_data_->Url() == response.Url()) {
      tainting = FetchRequestData::kBasicTainting;
    } else {
      switch (fetch_request_data_->Mode()) {
        case FetchRequestMode::kNoCORS:
          tainting = FetchRequestData::kOpaqueTainting;
          break;
        case FetchRequestMode::kSameOrigin:
        case FetchRequestMode::kCORS:
        case FetchRequestMode::kCORSWithForcedPreflight:
        case FetchRequestMode::kNavigate:
          PerformNetworkError("Fetch API cannot load " +
                              fetch_request_data_->Url().GetString() +
                              ". Redirects to data: URL are allowed only when "
                              "mode is \"no-cors\".");
          return;
      }
    }
  } else if (!SecurityOrigin::Create(response.Url())
                  ->IsSameSchemeHostPort(fetch_request_data_->Origin().get())) {
    switch (fetch_request_data_->Mode()) {
      case FetchRequestMode::kSameOrigin:
        NOTREACHED();
        break;
      case FetchRequestMode::kNoCORS:
        tainting = FetchRequestData::kOpaqueTainting;
        break;
      case FetchRequestMode::kCORS:
      case FetchRequestMode::kCORSWithForcedPreflight:
        tainting = FetchRequestData::kCORSTainting;
        break;
      case FetchRequestMode::kNavigate:
        LOG(FATAL);
        break;
    }
  }
  if (response.WasFetchedViaServiceWorker()) {
    switch (response.ResponseTypeViaServiceWorker()) {
      case FetchResponseType::kBasic:
      case FetchResponseType::kDefault:
        tainting = FetchRequestData::kBasicTainting;
        break;
      case FetchResponseType::kCORS:
        tainting = FetchRequestData::kCORSTainting;
        break;
      case FetchResponseType::kOpaque:
        tainting = FetchRequestData::kOpaqueTainting;
        break;
      case FetchResponseType::kOpaqueRedirect:
        DCHECK(
            NetworkUtils::IsRedirectResponseCode(response_http_status_code_));
        break;   
      case FetchResponseType::kError:
        LOG(FATAL) << "When ServiceWorker respond to the request from fetch() "
                      "with an error response, FetchManager::Loader::didFail() "
                      "must be called instead.";
        break;
    }
  }

  FetchResponseData* response_data = nullptr;
  SRIBytesConsumer* sri_consumer = nullptr;
  if (fetch_request_data_->Integrity().IsEmpty()) {
    response_data = FetchResponseData::CreateWithBuffer(new BodyStreamBuffer(
        script_state,
        new BytesConsumerForDataConsumerHandle(
            ExecutionContext::From(script_state), std::move(handle)),
        signal_));
  } else {
    sri_consumer = new SRIBytesConsumer();
    response_data = FetchResponseData::CreateWithBuffer(
        new BodyStreamBuffer(script_state, sri_consumer, signal_));
  }
  response_data->SetStatus(response.HttpStatusCode());
  response_data->SetStatusMessage(response.HttpStatusText());
  for (auto& it : response.HttpHeaderFields())
    response_data->HeaderList()->Append(it.key, it.value);
  if (response.UrlListViaServiceWorker().IsEmpty()) {
    response_data->SetURLList(url_list_);
  } else {
    DCHECK(response.WasFetchedViaServiceWorker());
    response_data->SetURLList(response.UrlListViaServiceWorker());
  }
  response_data->SetMIMEType(response.MimeType());
  response_data->SetResponseTime(response.ResponseTime());

  FetchResponseData* tainted_response = nullptr;

  DCHECK(!(NetworkUtils::IsRedirectResponseCode(response_http_status_code_) &&
           HasNonEmptyLocationHeader(response_data->HeaderList()) &&
           fetch_request_data_->Redirect() != FetchRedirectMode::kManual));

  if (NetworkUtils::IsRedirectResponseCode(response_http_status_code_) &&
      fetch_request_data_->Redirect() == FetchRedirectMode::kManual) {
    tainted_response = response_data->CreateOpaqueRedirectFilteredResponse();
  } else {
    switch (tainting) {
      case FetchRequestData::kBasicTainting:
        tainted_response = response_data->CreateBasicFilteredResponse();
        break;
      case FetchRequestData::kCORSTainting: {
        WebHTTPHeaderSet header_names =
            WebCORS::ExtractCorsExposedHeaderNamesList(
                fetch_request_data_->Credentials(),
                WrappedResourceResponse(response));
        tainted_response =
            response_data->CreateCORSFilteredResponse(header_names);
        break;
      }
      case FetchRequestData::kOpaqueTainting:
        tainted_response = response_data->CreateOpaqueFilteredResponse();
        break;
    }
  }

  Response* r =
      Response::Create(resolver_->GetExecutionContext(), tainted_response);
  if (response.Url().ProtocolIsData()) {
    r->headers()->HeaderList()->Remove(HTTPNames::Access_Control_Allow_Origin);
  }
  r->headers()->SetGuard(Headers::kImmutableGuard);

  if (fetch_request_data_->Integrity().IsEmpty()) {
    resolver_->Resolve(r);
    resolver_.Clear();
  } else {
    DCHECK(!integrity_verifier_);
    integrity_verifier_ = new SRIVerifier(
        std::move(handle), sri_consumer, r, this,
        fetch_request_data_->Integrity(), response.Url(),
        r->GetResponse()->GetType(),
        resolver_->GetExecutionContext()->GetTaskRunner(TaskType::kNetworking));
  }
}
