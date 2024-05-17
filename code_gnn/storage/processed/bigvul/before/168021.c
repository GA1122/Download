void FrameLoader::StartLoad(FrameLoadRequest& frame_load_request,
                            WebFrameLoadType type,
                            NavigationPolicy navigation_policy,
                            HistoryItem* history_item,
                            bool check_with_client) {
  DCHECK(Client()->HasWebView());
  ResourceRequest& resource_request = frame_load_request.GetResourceRequest();
  WebNavigationType navigation_type = DetermineNavigationType(
      type, resource_request.HttpBody() || frame_load_request.Form(),
      frame_load_request.TriggeringEventInfo() !=
          WebTriggeringEventInfo::kNotFromEvent);
  resource_request.SetRequestContext(
      DetermineRequestContextFromNavigationType(navigation_type));
  resource_request.SetFrameType(
      frame_->IsMainFrame() ? network::mojom::RequestContextFrameType::kTopLevel
                            : network::mojom::RequestContextFrameType::kNested);
  Document* origin_document = frame_load_request.OriginDocument();

  if (origin_document && origin_document->GetContentSecurityPolicy()
                             ->ExperimentalFeaturesEnabled()) {
    WebContentSecurityPolicyList initiator_csp =
        origin_document->GetContentSecurityPolicy()
            ->ExposeForNavigationalChecks();
    resource_request.SetInitiatorCSP(initiator_csp);
  }

  bool had_placeholder_client_document_loader =
      provisional_document_loader_ && !provisional_document_loader_->DidStart();

  RecordLatestRequiredCSP();



  ModifyRequestForCSP(resource_request, origin_document);

  navigation_policy = ShouldContinueForNavigationPolicy(
      resource_request, origin_document, frame_load_request.GetSubstituteData(),
      nullptr, frame_load_request.ShouldCheckMainWorldContentSecurityPolicy(),
      navigation_type, navigation_policy, type,
      frame_load_request.ClientRedirect() ==
          ClientRedirectPolicy::kClientRedirect,
      frame_load_request.TriggeringEventInfo(), frame_load_request.Form(),
      frame_load_request.GetBlobURLToken(), check_with_client);

  if (navigation_policy == kNavigationPolicyIgnore) {
    if (had_placeholder_client_document_loader &&
        !resource_request.CheckForBrowserSideNavigation()) {
      DetachDocumentLoader(provisional_document_loader_);
    }
    return;
  }

  if (had_placeholder_client_document_loader)
    provisional_document_loader_->SetSentDidFinishLoad();
  frame_->GetDocument()->CancelParsing();

  if (!had_placeholder_client_document_loader &&
      type == WebFrameLoadType::kStandard &&
      (navigation_policy == kNavigationPolicyCurrentTab ||
       navigation_policy == kNavigationPolicyHandledByClient)) {
    frame_->GetDocument()->CheckCompleted();
  }
  DetachDocumentLoader(provisional_document_loader_);

  if (!frame_->GetPage())
    return;

  progress_tracker_->ProgressStarted();
  if (navigation_policy == kNavigationPolicyHandledByClientForInitialHistory)
    return;
  DCHECK(navigation_policy == kNavigationPolicyCurrentTab ||
         navigation_policy == kNavigationPolicyHandledByClient);

  provisional_document_loader_ = CreateDocumentLoader(
      resource_request, frame_load_request, type, navigation_type);

  if (!had_placeholder_client_document_loader ||
      navigation_policy == kNavigationPolicyHandledByClient) {
    frame_->GetNavigationScheduler().Cancel();
  }

  if (frame_load_request.Form())
    Client()->DispatchWillSubmitForm(frame_load_request.Form());

  provisional_document_loader_->AppendRedirect(
      provisional_document_loader_->Url());

  if (IsBackForwardLoadType(type)) {
    DCHECK(history_item);
    provisional_document_loader_->SetItemForHistoryNavigation(history_item);
  }

  frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());

  Client()->DispatchDidStartProvisionalLoad(provisional_document_loader_,
                                            resource_request);
  DCHECK(provisional_document_loader_);

  if (navigation_policy == kNavigationPolicyCurrentTab) {
    provisional_document_loader_->StartLoading();
    probe::frameClearedScheduledClientNavigation(frame_);
  } else {
    probe::frameScheduledClientNavigation(frame_);
  }

  TakeObjectSnapshot();
}
