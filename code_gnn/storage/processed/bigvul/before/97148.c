static ResourceRequest::TargetType DetermineTargetTypeFromLoader(
    DocumentLoader* loader) {
  if (loader == loader->frameLoader()->provisionalDocumentLoader()) {
    if (loader->frameLoader()->isLoadingMainFrame()) {
      return ResourceRequest::TargetIsMainFrame;
    } else {
      return ResourceRequest::TargetIsSubFrame;
    }
  }
  return ResourceRequest::TargetIsSubResource;
}
