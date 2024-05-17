void FrameLoader::CommitNavigation(const FrameLoadRequest& passed_request,
                                   WebFrameLoadType frame_load_type,
                                   HistoryItem* history_item) {
  CHECK(!passed_request.OriginDocument());
  CHECK(passed_request.FrameName().IsEmpty());
  CHECK(!passed_request.Form());
  CHECK(passed_request.TriggeringEventInfo() ==
        WebTriggeringEventInfo::kNotFromEvent);

  DCHECK(frame_->GetDocument());
  DCHECK(!in_stop_all_loaders_);
  DCHECK(frame_->IsNavigationAllowed());
  DCHECK_EQ(Document::kNoDismissal,
            frame_->GetDocument()->PageDismissalEventBeingDispatched());

  if (HTMLFrameOwnerElement* element = frame_->DeprecatedLocalOwner())
    element->CancelPendingLazyLoad();

  FrameLoadRequest request(passed_request);
  request.GetResourceRequest().SetHasUserGesture(
      Frame::HasTransientUserActivation(frame_));

  if (frame_load_type == WebFrameLoadType::kStandard)
    frame_load_type = DetermineFrameLoadType(request);

  StartLoad(request, frame_load_type, kNavigationPolicyCurrentTab, history_item,
            false  );
}
