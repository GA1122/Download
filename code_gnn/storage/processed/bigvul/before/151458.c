bool FrameFetchContext::UpdateTimingInfoForIFrameNavigation(
    ResourceTimingInfo* info) {
  if (IsDetached())
    return false;

  if (!GetFrame()->DeprecatedLocalOwner() ||
      GetFrame()->DeprecatedLocalOwner()->LoadedNonEmptyDocument())
    return false;
  GetFrame()->DeprecatedLocalOwner()->DidLoadNonEmptyDocument();
  if (MasterDocumentLoader()->LoadType() == kFrameLoadTypeInitialHistoryLoad)
    return false;
  info->SetInitiatorType(GetFrame()->DeprecatedLocalOwner()->localName());
  return true;
}
