void RenderViewImpl::didCreateDataSource(WebFrame* frame, WebDataSource* ds) {
  DocumentState* document_state = DocumentState::FromDataSource(ds);
  if (!document_state) {
    document_state = new DocumentState;
    ds->setExtraData(document_state);
  }

  bool content_initiated = !pending_navigation_params_.get();
  if (content_initiated)
    document_state->set_navigation_state(
        NavigationState::CreateContentInitiated());
  else
    PopulateStateFromPendingNavigationParams(document_state);

  if (webview()) {
    if (WebFrame* old_frame = webview()->mainFrame()) {
      const WebURLRequest& original_request = ds->originalRequest();
      const GURL referrer(
          original_request.httpHeaderField(WebString::fromUTF8("Referer")));
      if (!referrer.is_empty() &&
          DocumentState::FromDataSource(
              old_frame->dataSource())->was_prefetcher()) {
        for (; old_frame; old_frame = old_frame->traverseNext(false)) {
          WebDataSource* old_frame_ds = old_frame->dataSource();
          if (old_frame_ds && referrer == GURL(old_frame_ds->request().url())) {
            document_state->set_was_referred_by_prefetcher(true);
            break;
          }
        }
      }
    }
  }

  if (content_initiated) {
    const WebURLRequest& request = ds->request();
    switch (request.cachePolicy()) {
      case WebURLRequest::UseProtocolCachePolicy:   
        document_state->set_load_type(DocumentState::LINK_LOAD_NORMAL);
        break;
      case WebURLRequest::ReloadIgnoringCacheData:   
        document_state->set_load_type(DocumentState::LINK_LOAD_RELOAD);
        break;
      case WebURLRequest::ReturnCacheDataElseLoad:   
        document_state->set_load_type(
            DocumentState::LINK_LOAD_CACHE_STALE_OK);
        break;
      case WebURLRequest::ReturnCacheDataDontLoad:   
        document_state->set_load_type(DocumentState::LINK_LOAD_CACHE_ONLY);
        break;
    }
  }

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidCreateDataSource(frame, ds));
}
