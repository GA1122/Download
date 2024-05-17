void DocumentLoader::ResponseReceived(
    Resource* resource,
    const ResourceResponse& response,
    std::unique_ptr<WebDataConsumerHandle> handle) {
  DCHECK_EQ(main_resource_, resource);
  DCHECK(!handle);
  DCHECK(frame_);

  application_cache_host_->DidReceiveResponseForMainResource(response);

  if (response.AppCacheID())
    GetMemoryCache()->Remove(main_resource_.Get());

  content_security_policy_ = ContentSecurityPolicy::Create();
  content_security_policy_->SetOverrideURLForSelf(response.Url());
  content_security_policy_->DidReceiveHeaders(
      ContentSecurityPolicyResponseHeaders(response));
  if (!content_security_policy_->AllowAncestors(frame_, response.Url())) {
    CancelLoadAfterCSPDenied(response);
    return;
  }

  if (RuntimeEnabledFeatures::EmbedderCSPEnforcementEnabled() &&
      !GetFrameLoader().RequiredCSP().IsEmpty()) {
    SecurityOrigin* parent_security_origin =
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
            MainResourceIdentifier());
        frame_->GetDocument()->AddConsoleMessage(console_message);
        CancelLoadAfterCSPDenied(response);
        return;
      }
    }
  }

  DCHECK(!frame_->GetPage()->Paused());

  if (response.DidServiceWorkerNavigationPreload())
    UseCounter::Count(frame_, WebFeature::kServiceWorkerNavigationPreload);
  response_ = response;

  if (IsArchiveMIMEType(response_.MimeType()) &&
      main_resource_->GetDataBufferingPolicy() != kBufferData)
    main_resource_->SetDataBufferingPolicy(kBufferData);

  if (!ShouldContinueForResponse()) {
    probe::ContinueWithPolicyIgnore(frame_, this, main_resource_->Identifier(),
                                    response_, main_resource_.Get());
    fetcher_->StopFetching();
    return;
  }

  if (frame_->Owner() && response_.IsHTTP() &&
      !FetchUtils::IsOkStatus(response_.HttpStatusCode()))
    frame_->Owner()->RenderFallbackContent();
}
