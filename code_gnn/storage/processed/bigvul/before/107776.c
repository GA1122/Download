void FrameLoader::StartNavigation(const FrameLoadRequest& passed_request,
                                  WebFrameLoadType frame_load_type,
                                  NavigationPolicy policy) {
  CHECK(!IsBackForwardLoadType(frame_load_type));
  DCHECK(passed_request.TriggeringEventInfo() !=
         WebTriggeringEventInfo::kUnknown);

  DCHECK(frame_->GetDocument());
  if (HTMLFrameOwnerElement* element = frame_->DeprecatedLocalOwner())
    element->CancelPendingLazyLoad();

  if (in_stop_all_loaders_)
    return;

  FrameLoadRequest request(passed_request);
  ResourceRequest& resource_request = request.GetResourceRequest();
  const KURL& url = resource_request.Url();
  Document* origin_document = request.OriginDocument();

  resource_request.SetHasUserGesture(
      LocalFrame::HasTransientUserActivation(frame_));

  if (!PrepareRequestForThisFrame(request))
    return;

  Frame* target_frame =
      request.Form() ? nullptr
                     : frame_->FindFrameForNavigation(
                           AtomicString(request.FrameName()), *frame_, url);

  bool should_navigate_target_frame = policy == kNavigationPolicyCurrentTab;

  if (target_frame && target_frame != frame_ && should_navigate_target_frame) {
    auto* target_local_frame = DynamicTo<LocalFrame>(target_frame);
    if (target_local_frame && !target_local_frame->IsNavigationAllowed()) {
      return;
    }

    bool was_in_same_page = target_frame->GetPage() == frame_->GetPage();

    request.SetFrameName("_self");
    target_frame->Navigate(request, frame_load_type);
    Page* page = target_frame->GetPage();
    if (!was_in_same_page && page)
      page->GetChromeClient().Focus(frame_);
    return;
  }

  SetReferrerForFrameRequest(request);

  if (!target_frame && !request.FrameName().IsEmpty()) {
    if (policy == kNavigationPolicyDownload) {
      Client()->DownloadURL(resource_request,
                            DownloadCrossOriginRedirects::kFollow);
      return;   
    } else if (should_navigate_target_frame) {
      resource_request.SetFrameType(
          network::mojom::RequestContextFrameType::kAuxiliary);
      CreateWindowForRequest(request, *frame_);
      return;   
    }
  }

  if (!frame_->IsNavigationAllowed() ||
      frame_->GetDocument()->PageDismissalEventBeingDispatched() !=
          Document::kNoDismissal) {
    return;
  }

  frame_load_type = DetermineFrameLoadType(
      resource_request.Url(), resource_request.HttpMethod(), origin_document,
      KURL(), frame_load_type);

  bool same_document_navigation =
      policy == kNavigationPolicyCurrentTab &&
      ShouldPerformFragmentNavigation(
          request.Form(), resource_request.HttpMethod(), frame_load_type, url);

  if (same_document_navigation) {
    CommitSameDocumentNavigation(
        url, frame_load_type, nullptr, request.ClientRedirect(),
        origin_document,
        request.TriggeringEventInfo() != WebTriggeringEventInfo::kNotFromEvent,
        nullptr  );
    return;
  }

  WebNavigationType navigation_type = DetermineNavigationType(
      frame_load_type, resource_request.HttpBody() || request.Form(),
      request.TriggeringEventInfo() != WebTriggeringEventInfo::kNotFromEvent);
  resource_request.SetRequestContext(
      DetermineRequestContextFromNavigationType(navigation_type));
  resource_request.SetFrameType(
      frame_->IsMainFrame() ? network::mojom::RequestContextFrameType::kTopLevel
                            : network::mojom::RequestContextFrameType::kNested);

  mojom::blink::NavigationInitiatorPtr navigation_initiator;
  WebContentSecurityPolicyList initiator_csp;
  if (origin_document && origin_document->GetContentSecurityPolicy()
                             ->ExperimentalFeaturesEnabled()) {
    initiator_csp = origin_document->GetContentSecurityPolicy()
                        ->ExposeForNavigationalChecks();
    auto mojo_request = mojo::MakeRequest(&navigation_initiator);
    origin_document->BindNavigationInitiatorRequest(std::move(mojo_request));
  }

  if (origin_document && origin_document->GetContentSecurityPolicy()) {
    last_origin_document_csp_ = ContentSecurityPolicy::Create();
    last_origin_document_csp_->CopyStateFrom(
        origin_document->GetContentSecurityPolicy());
    last_origin_document_csp_->CopyPluginTypesFrom(
        origin_document->GetContentSecurityPolicy());
  }

  RecordLatestRequiredCSP();



  ModifyRequestForCSP(resource_request, origin_document);

  DCHECK(Client()->HasWebView());
  if (url.PotentiallyDanglingMarkup() && url.ProtocolIsInHTTPFamily()) {
    Deprecation::CountDeprecation(
        origin_document, WebFeature::kCanRequestURLHTTPContainingNewline);
    return;
  }

  bool has_transient_activation =
      LocalFrame::HasTransientUserActivation(frame_);
  if (frame_->IsMainFrame() && origin_document &&
      frame_->GetPage() == origin_document->GetPage()) {
    LocalFrame::ConsumeTransientUserActivation(frame_);
  }

  Client()->BeginNavigation(
      resource_request, origin_document, nullptr  ,
      navigation_type, policy, has_transient_activation, frame_load_type,
      request.ClientRedirect() == ClientRedirectPolicy::kClientRedirect,
      request.TriggeringEventInfo(), request.Form(),
      request.ShouldCheckMainWorldContentSecurityPolicy(),
      request.GetBlobURLToken(), request.GetInputStartTime(),
      request.HrefTranslate().GetString(), std::move(initiator_csp),
      std::move(navigation_initiator));
}