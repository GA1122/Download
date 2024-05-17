bool FrameLoader::CreatePlaceholderDocumentLoader(
    const ResourceRequest& resource_request,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token,
    WebFrameLoadType frame_load_type,
    WebNavigationType navigation_type,
    std::unique_ptr<WebNavigationParams> navigation_params,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  if (!CancelProvisionalLoaderForNewNavigation(
          true  )) {
    return false;
  }

  provisional_document_loader_ = CreateDocumentLoader(
      resource_request, SubstituteData(), client_redirect_policy,
      devtools_navigation_token, frame_load_type, navigation_type,
      std::move(navigation_params), std::move(extra_data));
  provisional_document_loader_->AppendRedirect(
      provisional_document_loader_->Url());
  frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());
  probe::didStartProvisionalLoad(frame_);
  virtual_time_pauser_.PauseVirtualTime();
  TakeObjectSnapshot();
  return true;
}
