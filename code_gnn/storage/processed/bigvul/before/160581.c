void RenderFrameImpl::DidCreateDocumentLoader(
    blink::WebDocumentLoader* document_loader) {
  bool content_initiated = !pending_navigation_params_.get();

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(document_loader);
  if (!document_state) {
    document_state = new DocumentState;
    document_loader->SetExtraData(document_state);
    if (!content_initiated)
      PopulateDocumentStateFromPending(document_state);
  }

  blink::WebView* webview = render_view_->webview();
  if (content_initiated && webview && webview->MainFrame() &&
      webview->MainFrame()->IsWebLocalFrame() &&
      webview->MainFrame()->ToWebLocalFrame()->GetDocumentLoader()) {
    DocumentState* old_document_state = DocumentState::FromDocumentLoader(
        webview->MainFrame()->ToWebLocalFrame()->GetDocumentLoader());
    if (old_document_state) {
      InternalDocumentStateData* internal_data =
          InternalDocumentStateData::FromDocumentState(document_state);
      InternalDocumentStateData* old_internal_data =
          InternalDocumentStateData::FromDocumentState(old_document_state);
      internal_data->set_is_overriding_user_agent(
          old_internal_data->is_overriding_user_agent());
    }
  }

  UpdateNavigationState(document_state, false  ,
                        content_initiated);

  NavigationStateImpl* navigation_state = static_cast<NavigationStateImpl*>(
      document_state->navigation_state());

  document_loader->SetNavigationStartTime(
      ConvertToBlinkTime(navigation_state->common_params().navigation_start));

  if (!navigation_state->request_params()
           .navigation_timing.fetch_start.is_null()) {
    double redirect_start = ConvertToBlinkTime(
        navigation_state->request_params().navigation_timing.redirect_start);
    double redirect_end = ConvertToBlinkTime(
        navigation_state->request_params().navigation_timing.redirect_end);
    double fetch_start = ConvertToBlinkTime(
        navigation_state->request_params().navigation_timing.fetch_start);

    document_loader->UpdateNavigation(
        redirect_start, redirect_end, fetch_start,
        !navigation_state->request_params().redirects.empty());
  }

  if (navigation_state->common_params().source_location.has_value()) {
    blink::WebSourceLocation source_location;
    source_location.url = WebString::FromLatin1(
        navigation_state->common_params().source_location->url);
    source_location.line_number =
        navigation_state->common_params().source_location->line_number;
    source_location.column_number =
        navigation_state->common_params().source_location->column_number;
    document_loader->SetSourceLocation(source_location);
  }

  if (navigation_state->request_params().was_activated)
    document_loader->SetUserActivated();

  if (document_loader->GetServiceWorkerNetworkProvider())
    return;

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  scoped_refptr<SharedURLLoaderFactory> direct_network_loader_factory;
  if (render_thread) {
    direct_network_loader_factory =
        base::MakeRefCounted<PossiblyAssociatedWrapperSharedURLLoaderFactory>(
            render_thread->blink_platform_impl()
                ->CreateNetworkURLLoaderFactory());
  }
  document_loader->SetServiceWorkerNetworkProvider(
      ServiceWorkerNetworkProvider::CreateForNavigation(
          routing_id_, navigation_state->request_params(), frame_,
          content_initiated, std::move(controller_service_worker_info_),
          std::move(direct_network_loader_factory)));
}
