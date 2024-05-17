WebFrameLoadType FrameLoader::DetermineFrameLoadType(
    const KURL& url,
    const AtomicString& http_method,
    Document* origin_document,
    const KURL& failing_url,
    WebFrameLoadType frame_load_type) {
  if (frame_load_type == WebFrameLoadType::kStandard ||
      frame_load_type == WebFrameLoadType::kReplaceCurrentItem) {
    if (frame_->Tree().Parent() &&
        !state_machine_.CommittedFirstRealDocumentLoad())
      return WebFrameLoadType::kReplaceCurrentItem;
    if (!frame_->Tree().Parent() && !Client()->BackForwardLength()) {
      if (Opener() && url.IsEmpty())
        return WebFrameLoadType::kReplaceCurrentItem;
      return WebFrameLoadType::kStandard;
    }
  }
  if (frame_load_type != WebFrameLoadType::kStandard)
    return frame_load_type;
  if ((!state_machine_.CommittedMultipleRealLoads() &&
       DeprecatedEqualIgnoringCase(frame_->GetDocument()->Url(), BlankURL())))
    return WebFrameLoadType::kReplaceCurrentItem;

  if (url == document_loader_->UrlForHistory()) {
    if (http_method == http_names::kPOST)
      return WebFrameLoadType::kStandard;
    if (!origin_document)
      return WebFrameLoadType::kReload;
    return WebFrameLoadType::kReplaceCurrentItem;
  }

  if (failing_url == document_loader_->UrlForHistory() &&
      document_loader_->LoadType() == WebFrameLoadType::kReload)
    return WebFrameLoadType::kReload;

  if (url.IsEmpty() && failing_url.IsEmpty()) {
    return WebFrameLoadType::kReplaceCurrentItem;
  }

  if (origin_document && !origin_document->CanCreateHistoryEntry())
    return WebFrameLoadType::kReplaceCurrentItem;

  return WebFrameLoadType::kStandard;
}
