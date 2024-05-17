void WebLocalFrameImpl::LoadData(const WebData& data,
                                 const WebString& mime_type,
                                 const WebString& text_encoding,
                                 const WebURL& base_url,
                                 const WebURL& unreachable_url,
                                 bool replace,
                                 WebFrameLoadType web_frame_load_type,
                                 const WebHistoryItem& item,
                                 WebHistoryLoadType web_history_load_type,
                                 bool is_client_redirect) {
  DCHECK(GetFrame());

  ResourceRequest request;
  HistoryItem* history_item = item;
  DocumentLoader* provisional_document_loader =
      GetFrame()->Loader().GetProvisionalDocumentLoader();
  if (replace && !unreachable_url.IsEmpty() && provisional_document_loader) {
    request = provisional_document_loader->OriginalRequest();
    if (provisional_document_loader->LoadType() == kFrameLoadTypeBackForward) {
      history_item = provisional_document_loader->GetHistoryItem();
      web_frame_load_type = WebFrameLoadType::kBackForward;
    }
  }
  request.SetURL(base_url);
  request.SetCheckForBrowserSideNavigation(false);

  FrameLoadRequest frame_request(
      0, request,
      SubstituteData(data, mime_type, text_encoding, unreachable_url));
  DCHECK(frame_request.GetSubstituteData().IsValid());
  frame_request.SetReplacesCurrentItem(replace);
  if (is_client_redirect)
    frame_request.SetClientRedirect(ClientRedirectPolicy::kClientRedirect);

  GetFrame()->Loader().Load(
      frame_request, static_cast<FrameLoadType>(web_frame_load_type),
      history_item, static_cast<HistoryLoadType>(web_history_load_type));
}
