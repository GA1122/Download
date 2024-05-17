FrameLoadType FrameLoader::DetermineFrameLoadType(
    const FrameLoadRequest& request) {
  if (frame_->Tree().Parent() &&
      !state_machine_.CommittedFirstRealDocumentLoad())
    return kFrameLoadTypeInitialInChildFrame;
  if (!frame_->Tree().Parent() && !Client()->BackForwardLength()) {
    if (Opener() && request.GetResourceRequest().Url().IsEmpty())
      return kFrameLoadTypeReplaceCurrentItem;
    return kFrameLoadTypeStandard;
  }
  if (request.GetResourceRequest().GetCacheMode() ==
      mojom::FetchCacheMode::kValidateCache)
    return kFrameLoadTypeReload;
  if (request.GetResourceRequest().GetCacheMode() ==
      mojom::FetchCacheMode::kBypassCache)
    return kFrameLoadTypeReloadBypassingCache;
  if (request.ReplacesCurrentItem() ||
      (!state_machine_.CommittedMultipleRealLoads() &&
       DeprecatedEqualIgnoringCase(frame_->GetDocument()->Url(), BlankURL())))
    return kFrameLoadTypeReplaceCurrentItem;

  if (request.GetResourceRequest().Url() == document_loader_->UrlForHistory()) {
    if (request.GetResourceRequest().HttpMethod() == HTTPNames::POST)
      return kFrameLoadTypeStandard;
    if (!request.OriginDocument())
      return kFrameLoadTypeReload;
    return kFrameLoadTypeReplaceCurrentItem;
  }

  if (request.GetSubstituteData().FailingURL() ==
          document_loader_->UrlForHistory() &&
      document_loader_->LoadType() == kFrameLoadTypeReload)
    return kFrameLoadTypeReload;

  if (request.GetResourceRequest().Url().IsEmpty() &&
      request.GetSubstituteData().FailingURL().IsEmpty()) {
    return kFrameLoadTypeReplaceCurrentItem;
  }

  if (request.OriginDocument() &&
      !request.OriginDocument()->CanCreateHistoryEntry())
    return kFrameLoadTypeReplaceCurrentItem;

  return kFrameLoadTypeStandard;
}
