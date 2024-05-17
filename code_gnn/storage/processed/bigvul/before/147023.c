void WebLocalFrameImpl::Load(const WebURLRequest& request,
                             WebFrameLoadType web_frame_load_type,
                             const WebHistoryItem& item,
                             WebHistoryLoadType web_history_load_type,
                             bool is_client_redirect) {
  DCHECK(GetFrame());
  DCHECK(!request.IsNull());
  const ResourceRequest& resource_request = request.ToResourceRequest();

  if (resource_request.Url().ProtocolIs("javascript") &&
      web_frame_load_type == WebFrameLoadType::kStandard) {
    LoadJavaScriptURL(resource_request.Url());
    return;
  }

  if (text_finder_)
    text_finder_->ClearActiveFindMatch();

  FrameLoadRequest frame_request = FrameLoadRequest(nullptr, resource_request);
  if (is_client_redirect)
    frame_request.SetClientRedirect(ClientRedirectPolicy::kClientRedirect);
  HistoryItem* history_item = item;
  GetFrame()->Loader().Load(
      frame_request, static_cast<FrameLoadType>(web_frame_load_type),
      history_item, static_cast<HistoryLoadType>(web_history_load_type));
}
