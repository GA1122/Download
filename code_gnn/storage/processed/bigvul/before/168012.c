WebFrameLoadType FrameLoader::DetermineFrameLoadType(
    const FrameLoadRequest& request) {
  if (frame_->Tree().Parent() &&
      !state_machine_.CommittedFirstRealDocumentLoad())
    return WebFrameLoadType::kInitialInChildFrame;
  if (!frame_->Tree().Parent() && !Client()->BackForwardLength()) {
    if (Opener() && request.GetResourceRequest().Url().IsEmpty())
      return WebFrameLoadType::kReplaceCurrentItem;
    return WebFrameLoadType::kStandard;
  }
  if (request.GetResourceRequest().GetCacheMode() ==
      mojom::FetchCacheMode::kValidateCache)
    return WebFrameLoadType::kReload;
  if (request.GetResourceRequest().GetCacheMode() ==
      mojom::FetchCacheMode::kBypassCache)
    return WebFrameLoadType::kReloadBypassingCache;
  if (request.ReplacesCurrentItem() ||
      (!state_machine_.CommittedMultipleRealLoads() &&
       DeprecatedEqualIgnoringCase(frame_->GetDocument()->Url(), BlankURL())))
    return WebFrameLoadType::kReplaceCurrentItem;

  if (request.GetResourceRequest().Url() == document_loader_->UrlForHistory()) {
    if (request.GetResourceRequest().HttpMethod() == HTTPNames::POST)
      return WebFrameLoadType::kStandard;
    if (!request.OriginDocument())
      return WebFrameLoadType::kReload;
    return WebFrameLoadType::kReplaceCurrentItem;
  }

  if (request.GetSubstituteData().FailingURL() ==
          document_loader_->UrlForHistory() &&
      document_loader_->LoadType() == WebFrameLoadType::kReload)
    return WebFrameLoadType::kReload;

  if (request.GetResourceRequest().Url().IsEmpty() &&
      request.GetSubstituteData().FailingURL().IsEmpty()) {
    return WebFrameLoadType::kReplaceCurrentItem;
  }

  if (request.OriginDocument() &&
      !request.OriginDocument()->CanCreateHistoryEntry())
    return WebFrameLoadType::kReplaceCurrentItem;

  return WebFrameLoadType::kStandard;
}
