void DocumentLoader::ResponseReceived(
    Resource* resource,
    const ResourceResponse& response,
    std::unique_ptr<WebDataConsumerHandle> handle) {
  DCHECK_EQ(GetResource(), resource);
  DCHECK(!handle);
  DCHECK(frame_);

  application_cache_host_->DidReceiveResponseForMainResource(response);

  if (response.AppCacheID())
    GetMemoryCache()->Remove(resource);

  content_security_policy_ = ContentSecurityPolicy::Create();
  content_security_policy_->SetOverrideURLForSelf(response.Url());
  if (!frame_->GetSettings()->BypassCSP()) {
    content_security_policy_->DidReceiveHeaders(
        ContentSecurityPolicyResponseHeaders(response));

    if (!content_security_policy_->HasPolicyFromSource(
            kContentSecurityPolicyHeaderSourceOriginPolicy)) {
      std::unique_ptr<OriginPolicy> origin_policy = OriginPolicy::From(
          StringUTF8Adaptor(request_.GetOriginPolicy()).AsStringPiece());
      if (origin_policy) {
        for (auto csp : origin_policy->GetContentSecurityPolicies()) {
          content_security_policy_->DidReceiveHeader(
              WTF::String::FromUTF8(csp.policy.data(), csp.policy.length()),
              csp.report_only ? kContentSecurityPolicyHeaderTypeReport
                              : kContentSecurityPolicyHeaderTypeEnforce,
              kContentSecurityPolicyHeaderSourceOriginPolicy);
        }
      }
    }
  }
  if (!content_security_policy_->AllowAncestors(frame_, response.Url())) {
    CancelLoadAfterCSPDenied(response);
    return;
  }

  if (!frame_->GetSettings()->BypassCSP() &&
      !GetFrameLoader().RequiredCSP().IsEmpty()) {
    const SecurityOrigin* parent_security_origin =
        frame_->Tree().Parent()->GetSecurityContext()->GetSecurityOrigin();
    if (ContentSecurityPolicy::ShouldEnforceEmbeddersPolicy(
            response, parent_security_origin)) {
      content_security_policy_->AddPolicyFromHeaderValue(
          GetFrameLoader().RequiredCSP(),
          kContentSecurityPolicyHeaderTypeEnforce,
          kContentSecurityPolicyHeaderSourceHTTP);
    } else {
      ContentSecurityPolicy* required_csp = ContentSecurityPolicy::Create();
      required_csp->AddPolicyFromHeaderValue(
          GetFrameLoader().RequiredCSP(),
          kContentSecurityPolicyHeaderTypeEnforce,
          kContentSecurityPolicyHeaderSourceHTTP);
      if (!required_csp->Subsumes(*content_security_policy_)) {
        String message = "Refused to display '" +
                         response.Url().ElidedString() +
                         "' because it has not opted-into the following policy "
                         "required by its embedder: '" +
                         GetFrameLoader().RequiredCSP() + "'.";
        ConsoleMessage* console_message = ConsoleMessage::CreateForRequest(
            kSecurityMessageSource, kErrorMessageLevel, message, response.Url(),
            this, MainResourceIdentifier());
        frame_->GetDocument()->AddConsoleMessage(console_message);
        CancelLoadAfterCSPDenied(response);
        return;
      }
    }
  }

  DCHECK(!frame_->GetPage()->Paused());

  if (response.DidServiceWorkerNavigationPreload())
    UseCounter::Count(this, WebFeature::kServiceWorkerNavigationPreload);

  response_ = response;

  if (IsArchiveMIMEType(response_.MimeType()) &&
      resource->GetDataBufferingPolicy() != kBufferData)
    resource->SetDataBufferingPolicy(kBufferData);

  if (!ShouldContinueForResponse()) {
    probe::didReceiveResourceResponse(frame_->GetDocument(),
                                      resource->Identifier(), this, response_,
                                      resource);
    fetcher_->StopFetching();
    return;
  }

  if (frame_->Owner() && response_.IsHTTP() &&
      !CORS::IsOkStatus(response_.HttpStatusCode()))
    frame_->Owner()->RenderFallbackContent();
}
