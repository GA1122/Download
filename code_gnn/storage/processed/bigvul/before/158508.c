void WebLocalFrameImpl::CommitDataNavigation(
    const WebData& data,
    const WebString& mime_type,
    const WebString& text_encoding,
    const WebURL& base_url,
    const WebURL& unreachable_url,
    bool replace,
    WebFrameLoadType web_frame_load_type,
    const WebHistoryItem& item,
    bool is_client_redirect,
    std::unique_ptr<WebDocumentLoader::ExtraData> navigation_data,
    const WebURLRequest* original_failed_request,
    const WebNavigationTimings& navigation_timings) {
  DCHECK(GetFrame());

  ResourceRequest request;
  HistoryItem* history_item = item;
  DocumentLoader* provisional_document_loader =
      GetFrame()->Loader().GetProvisionalDocumentLoader();
  if (replace && !unreachable_url.IsEmpty() && provisional_document_loader) {
    request = provisional_document_loader->OriginalRequest();

    WebFrameLoadType previous_load_type =
        provisional_document_loader->LoadType();
    if (previous_load_type == WebFrameLoadType::kBackForward &&
        provisional_document_loader->GetHistoryItem()) {
      history_item = provisional_document_loader->GetHistoryItem();
      web_frame_load_type = WebFrameLoadType::kBackForward;
    } else if (previous_load_type == WebFrameLoadType::kReload ||
               previous_load_type == WebFrameLoadType::kReloadBypassingCache) {
      web_frame_load_type = previous_load_type;
    }
  } else if (original_failed_request) {
    request = original_failed_request->ToResourceRequest();

    request.SetCacheMode(mojom::FetchCacheMode::kNoStore);
  }
  request.SetURL(base_url);

  FrameLoadRequest frame_request(
      nullptr, request,
      SubstituteData(data, mime_type, text_encoding, unreachable_url));
  DCHECK(frame_request.GetSubstituteData().IsValid());
  frame_request.SetReplacesCurrentItem(replace);
  if (is_client_redirect)
    frame_request.SetClientRedirect(ClientRedirectPolicy::kClientRedirect);

  GetFrame()->Loader().CommitNavigation(
      frame_request, web_frame_load_type, history_item,
      std::move(navigation_data), navigation_timings);
}
