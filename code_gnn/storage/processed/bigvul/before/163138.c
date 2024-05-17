bool HTMLFrameOwnerElement::LoadOrRedirectSubframe(
    const KURL& url,
    const AtomicString& frame_name,
    bool replace_current_item) {
  UpdateContainerPolicy();

  if (ContentFrame()) {
    ContentFrame()->Navigate(GetDocument(), url, replace_current_item,
                             UserGestureStatus::kNone);
    return true;
  }

  if (!SubframeLoadingDisabler::CanLoadFrame(*this))
    return false;

  if (GetDocument().GetFrame()->GetPage()->SubframeCount() >=
      Page::kMaxNumberOfFrames)
    return false;

  LocalFrame* child_frame =
      GetDocument().GetFrame()->Client()->CreateFrame(frame_name, this);
  DCHECK_EQ(ContentFrame(), child_frame);
  if (!child_frame)
    return false;

  ResourceRequest request(url);
  ReferrerPolicy policy = ReferrerPolicyAttribute();
  if (policy != kReferrerPolicyDefault) {
    request.SetHTTPReferrer(SecurityPolicy::GenerateReferrer(
        policy, url, GetDocument().OutgoingReferrer()));
  }

  FrameLoadType child_load_type = kFrameLoadTypeInitialInChildFrame;
  if (!GetDocument().LoadEventFinished() &&
      GetDocument().Loader()->LoadType() ==
          kFrameLoadTypeReloadBypassingCache) {
    child_load_type = kFrameLoadTypeReloadBypassingCache;
    request.SetCacheMode(mojom::FetchCacheMode::kBypassCache);
  }

  child_frame->Loader().Load(FrameLoadRequest(&GetDocument(), request),
                             child_load_type);
  return true;
}
