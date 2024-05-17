void FrameLoader::LoadInternal(const FrameLoadRequest& passed_request,
                               FrameLoadType frame_load_type,
                               HistoryItem* history_item,
                               bool check_with_client) {
  DCHECK(frame_->GetDocument());

  if (HTMLFrameOwnerElement* element = frame_->DeprecatedLocalOwner())
    element->CancelPendingLazyLoad();

  if (IsBackForwardLoadType(frame_load_type) && !frame_->IsNavigationAllowed())
    return;

  if (in_stop_all_loaders_)
    return;

  FrameLoadRequest request(passed_request);
  request.GetResourceRequest().SetHasUserGesture(
      Frame::HasTransientUserActivation(frame_));

  if (!PrepareRequestForThisFrame(request))
    return;

  Frame* target_frame = request.Form()
                            ? nullptr
                            : frame_->FindFrameForNavigation(
                                  AtomicString(request.FrameName()), *frame_,
                                  request.GetResourceRequest().Url());

  NavigationPolicy policy = NavigationPolicyForRequest(request);
  if (target_frame && target_frame != frame_ &&
      ShouldNavigateTargetFrame(policy)) {
    if (target_frame->IsLocalFrame() &&
        !ToLocalFrame(target_frame)->IsNavigationAllowed()) {
      return;
    }

    bool was_in_same_page = target_frame->GetPage() == frame_->GetPage();

    request.SetFrameName("_self");
    target_frame->Navigate(request);
    Page* page = target_frame->GetPage();
    if (!was_in_same_page && page)
      page->GetChromeClient().Focus(frame_);
    return;
  }

  SetReferrerForFrameRequest(request);

  if (!target_frame && !request.FrameName().IsEmpty()) {
    if (policy == kNavigationPolicyDownload) {
      Client()->DownloadURL(request.GetResourceRequest());
      return;   
    } else if (ShouldNavigateTargetFrame(policy)) {
      request.GetResourceRequest().SetFrameType(
          network::mojom::RequestContextFrameType::kAuxiliary);
      CreateWindowForRequest(request, *frame_, policy);
      return;   
    }
  }

  if (!frame_->IsNavigationAllowed())
    return;

  const KURL& url = request.GetResourceRequest().Url();
  FrameLoadType new_load_type = (frame_load_type == kFrameLoadTypeStandard)
                                    ? DetermineFrameLoadType(request)
                                    : frame_load_type;

  bool same_document_navigation =
      policy == kNavigationPolicyCurrentTab &&
      ShouldPerformFragmentNavigation(request.Form(),
                                      request.GetResourceRequest().HttpMethod(),
                                      new_load_type, url);

  if (same_document_navigation) {
    CommitSameDocumentNavigation(
        request.GetResourceRequest().Url(), new_load_type, history_item,
        request.ClientRedirect(), request.OriginDocument(),
        request.TriggeringEvent());
    return;
  }

  StartLoad(request, new_load_type, policy, history_item, check_with_client);
}
