static SinglePageAppNavigationType CategorizeSinglePageAppNavigation(
    SameDocumentNavigationSource same_document_navigation_source,
    FrameLoadType frame_load_type) {
  switch (same_document_navigation_source) {
    case kSameDocumentNavigationDefault:
      if (frame_load_type == kFrameLoadTypeBackForward) {
        return kSPANavTypeSameDocumentBackwardOrForward;
      }
      return kSPANavTypeOtherFragmentNavigation;
    case kSameDocumentNavigationHistoryApi:
      DCHECK(frame_load_type != kFrameLoadTypeBackForward);
      return kSPANavTypeHistoryPushStateOrReplaceState;
  }
  NOTREACHED();
  return kSPANavTypeSameDocumentBackwardOrForward;
}
