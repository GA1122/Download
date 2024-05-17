void RenderFrameImpl::LoadNavigationErrorPage(
    WebDocumentLoader* document_loader,
    const WebURLError& error,
    const base::Optional<std::string>& error_page_content,
    bool replace_current_item,
    bool inherit_document_state) {
  WebURLRequest failed_request = document_loader->GetRequest();

  std::string error_html;
  if (error_page_content) {
    error_html = error_page_content.value();
  } else {
    GetContentClient()->renderer()->PrepareErrorPage(this, failed_request,
                                                     error, &error_html);
  }

  frame_->EnableViewSourceMode(false);

  auto navigation_params = std::make_unique<WebNavigationParams>();
  std::unique_ptr<DocumentState> document_state;

  if (inherit_document_state) {
    NavigationState* navigation_state =
        NavigationState::FromDocumentLoader(document_loader);
    document_state = BuildDocumentStateFromParams(
        navigation_state->common_params(), navigation_state->request_params(),
        base::TimeTicks(),   
        CommitNavigationCallback(), nullptr);
    FillNavigationParams(navigation_state->common_params(),
                         navigation_state->request_params(),
                         navigation_params.get());
  } else {
    document_state = BuildDocumentState();
  }

  failed_request.SetCacheMode(blink::mojom::FetchCacheMode::kNoStore);
  failed_request.SetURL(GURL(kUnreachableWebDataURL));
  navigation_params->request = failed_request;

  if (replace_current_item)
    navigation_params->frame_load_type = WebFrameLoadType::kReplaceCurrentItem;

  navigation_params->data = WebData(error_html.data(), error_html.length());
  navigation_params->mime_type = "text/html";
  navigation_params->text_encoding = "UTF-8";
  navigation_params->unreachable_url = error.url();

  navigation_params->service_worker_network_provider =
      BuildServiceWorkerNetworkProviderForNavigation(nullptr, nullptr);

  frame_->CommitNavigation(std::move(navigation_params),
                           std::move(document_state));
}
