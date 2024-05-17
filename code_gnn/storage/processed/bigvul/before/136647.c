DocumentLoader* FrameLoader::CreateDocumentLoader(
    const ResourceRequest& request,
    const FrameLoadRequest& frame_load_request,
    FrameLoadType load_type,
    NavigationType navigation_type) {
  DocumentLoader* loader = Client()->CreateDocumentLoader(
      frame_, request,
      frame_load_request.GetSubstituteData().IsValid()
          ? frame_load_request.GetSubstituteData()
          : DefaultSubstituteDataForURL(request.Url()),
      frame_load_request.ClientRedirect(),
      frame_load_request.GetDevToolsNavigationToken());

  loader->SetLoadType(load_type);
  loader->SetNavigationType(navigation_type);
  bool replace_current_item = load_type == kFrameLoadTypeReplaceCurrentItem &&
                              (!Opener() || !request.Url().IsEmpty());
  loader->SetReplacesCurrentHistoryItem(replace_current_item);

  probe::lifecycleEvent(frame_, loader, "init", CurrentTimeTicksInSeconds());
  return loader;
}
