void RenderFrameImpl::LoadNavigationErrorPage(
    WebDocumentLoader* document_loader,
    const WebURLError& error,
    const base::Optional<std::string>& error_page_content,
    bool replace_current_item,
    bool inherit_document_state) {
  std::string error_html;
  if (error_page_content) {
    error_html = error_page_content.value();
  } else {
    GetContentClient()->renderer()->PrepareErrorPage(
        this, error, document_loader->HttpMethod().Ascii(),
        false  , &error_html);
  }

  frame_->EnableViewSourceMode(false);

  auto navigation_params = WebNavigationParams::CreateForErrorPage(
      document_loader, error_html, GURL(kUnreachableWebDataURL), error.url(),
      error.reason());
  std::unique_ptr<DocumentState> document_state;

  if (inherit_document_state) {
    NavigationState* navigation_state =
        NavigationState::FromDocumentLoader(document_loader);
    document_state = BuildDocumentStateFromParams(
        navigation_state->common_params(), navigation_state->commit_params(),
        base::TimeTicks(),   
        mojom::FrameNavigationControl::CommitNavigationCallback(),
        mojom::NavigationClient::CommitNavigationCallback(), nullptr, nullptr,
        ResourceDispatcher::MakeRequestID(),
        false  );
    FillMiscNavigationParams(navigation_state->common_params(),
                             navigation_state->commit_params(),
                             navigation_params.get());
  } else {
    document_state = BuildDocumentState();
  }

  if (replace_current_item)
    navigation_params->frame_load_type = WebFrameLoadType::kReplaceCurrentItem;
  navigation_params->service_worker_network_provider =
      ServiceWorkerNetworkProviderForFrame::CreateInvalidInstance();

  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
}
