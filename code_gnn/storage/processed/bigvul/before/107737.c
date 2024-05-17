bool FrameLoader::CreatePlaceholderDocumentLoader(
    const WebNavigationInfo& info,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  if (!CancelProvisionalLoaderForNewNavigation(
          true  ,
          false  )) {
    return false;
  }

  auto navigation_params = std::make_unique<WebNavigationParams>();
  navigation_params->url = info.url_request.Url();
  navigation_params->frame_load_type = info.frame_load_type;
  navigation_params->is_client_redirect = info.is_client_redirect;
  navigation_params->navigation_timings.input_start = info.input_start;
  provisional_document_loader_ = Client()->CreateDocumentLoader(
      frame_, info.navigation_type, std::move(navigation_params),
      std::move(extra_data));
  frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());
  probe::DidStartProvisionalLoad(frame_);
  virtual_time_pauser_.PauseVirtualTime();
  TakeObjectSnapshot();
  return true;
}
