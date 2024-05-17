void FrameLoader::StartNavigation(const FrameLoadRequest& passed_request,
                                  WebFrameLoadType frame_load_type,
                                  NavigationPolicy policy) {
  CHECK(!passed_request.GetSubstituteData().IsValid());
  CHECK(!IsBackForwardLoadType(frame_load_type));
  DCHECK(passed_request.TriggeringEventInfo() !=
         WebTriggeringEventInfo::kUnknown);
  DCHECK(policy != kNavigationPolicyHandledByClient &&
         policy != kNavigationPolicyHandledByClientForInitialHistory);

  DCHECK(frame_->GetDocument());
  if (HTMLFrameOwnerElement* element = frame_->DeprecatedLocalOwner())
    element->CancelPendingLazyLoad();

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

  bool should_navigate_target_frame = policy == kNavigationPolicyCurrentTab;

  if (target_frame && target_frame != frame_ && should_navigate_target_frame) {
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
    } else if (should_navigate_target_frame) {
      request.GetResourceRequest().SetFrameType(
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

  const KURL& url = request.GetResourceRequest().Url();
  if (frame_load_type == WebFrameLoadType::kStandard)
    frame_load_type = DetermineFrameLoadType(request);

  bool same_document_navigation =
      policy == kNavigationPolicyCurrentTab &&
      ShouldPerformFragmentNavigation(request.Form(),
                                      request.GetResourceRequest().HttpMethod(),
                                      frame_load_type, url);

  if (same_document_navigation) {
    CommitSameDocumentNavigation(
        request.GetResourceRequest().Url(), frame_load_type, nullptr,
        request.ClientRedirect(), request.OriginDocument(),
        request.TriggeringEventInfo() != WebTriggeringEventInfo::kNotFromEvent);
    return;
  }

  StartLoad(request, frame_load_type, policy, nullptr,
            true  );
}
