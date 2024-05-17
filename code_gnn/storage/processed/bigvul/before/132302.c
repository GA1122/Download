void RenderFrameImpl::didCreateDataSource(blink::WebLocalFrame* frame,
                                          blink::WebDataSource* datasource) {
  DCHECK(!frame_ || frame_ == frame);

  bool content_initiated = !pending_navigation_params_.get();

  if (pending_navigation_params_.get()) {
    for (const auto& i :
         pending_navigation_params_->request_params.redirects) {
      datasource->appendRedirect(i);
    }
  }

  DocumentState* document_state = DocumentState::FromDataSource(datasource);
  if (!document_state) {
    document_state = new DocumentState;
    datasource->setExtraData(document_state);
    if (!content_initiated)
      PopulateDocumentStateFromPending(document_state);
  }

  blink::WebView* webview = render_view_->webview();
  if (content_initiated && webview && webview->mainFrame() &&
      webview->mainFrame()->isWebLocalFrame() &&
      webview->mainFrame()->dataSource()) {
    DocumentState* old_document_state =
        DocumentState::FromDataSource(webview->mainFrame()->dataSource());
    if (old_document_state) {
      InternalDocumentStateData* internal_data =
          InternalDocumentStateData::FromDocumentState(document_state);
      InternalDocumentStateData* old_internal_data =
          InternalDocumentStateData::FromDocumentState(old_document_state);
      internal_data->set_is_overriding_user_agent(
          old_internal_data->is_overriding_user_agent());
    }
  }

  if (content_initiated) {
    document_state->set_navigation_state(
        NavigationStateImpl::CreateContentInitiated());
  } else {
    document_state->set_navigation_state(CreateNavigationStateFromPending());
    pending_navigation_params_.reset();
  }

  if (webview) {
    if (WebFrame* old_frame = webview->mainFrame()) {
      const WebURLRequest& original_request = datasource->originalRequest();
      const GURL referrer(
          original_request.httpHeaderField(WebString::fromUTF8("Referer")));
      if (!referrer.is_empty() && old_frame->isWebLocalFrame() &&
          DocumentState::FromDataSource(old_frame->dataSource())
              ->was_prefetcher()) {
        for (; old_frame; old_frame = old_frame->traverseNext(false)) {
          WebDataSource* old_frame_datasource = old_frame->dataSource();
          if (old_frame_datasource &&
              referrer == GURL(old_frame_datasource->request().url())) {
            document_state->set_was_referred_by_prefetcher(true);
            break;
          }
        }
      }
    }
  }

  if (content_initiated) {
    const WebURLRequest& request = datasource->request();
    switch (request.cachePolicy()) {
      case WebURLRequest::UseProtocolCachePolicy:   
        document_state->set_load_type(DocumentState::LINK_LOAD_NORMAL);
        break;
      case WebURLRequest::ReloadIgnoringCacheData:   
      case WebURLRequest::ReloadBypassingCache:      
        document_state->set_load_type(DocumentState::LINK_LOAD_RELOAD);
        break;
      case WebURLRequest::ReturnCacheDataElseLoad:   
        document_state->set_load_type(DocumentState::LINK_LOAD_CACHE_STALE_OK);
        break;
      case WebURLRequest::ReturnCacheDataDontLoad:   
        document_state->set_load_type(DocumentState::LINK_LOAD_CACHE_ONLY);
        break;
      default:
        NOTREACHED();
    }
  }

  if (!ServiceWorkerNetworkProvider::FromDocumentState(
          DocumentState::FromDataSource(datasource))) {
    ServiceWorkerProviderType provider_type =
        SERVICE_WORKER_PROVIDER_FOR_WINDOW;
    if ((frame->effectiveSandboxFlags() & blink::WebSandboxFlags::Origin) ==
        blink::WebSandboxFlags::Origin) {
      provider_type = SERVICE_WORKER_PROVIDER_FOR_SANDBOXED_FRAME;
    }
    scoped_ptr<ServiceWorkerNetworkProvider> network_provider(
        new ServiceWorkerNetworkProvider(routing_id_, provider_type));
    ServiceWorkerNetworkProvider::AttachToDocumentState(
        DocumentState::FromDataSource(datasource),
        network_provider.Pass());
  }
}
