void FrameLoader::DetachDocumentLoader(Member<DocumentLoader>& loader) {
  if (!loader)
    return;

  FrameNavigationDisabler navigation_disabler(*frame_);
  loader->DetachFromFrame();
  loader = nullptr;
}
