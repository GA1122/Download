blink::mojom::CommitResult WebLocalFrameImpl::CommitSameDocumentNavigation(
    const WebURL& url,
    WebFrameLoadType web_frame_load_type,
    const WebHistoryItem& item,
    bool is_client_redirect) {
  DCHECK(GetFrame());
  DCHECK(!url.ProtocolIs("javascript"));

  HistoryItem* history_item = item;
  return GetFrame()->Loader().CommitSameDocumentNavigation(
      url, web_frame_load_type, history_item,
      is_client_redirect ? ClientRedirectPolicy::kClientRedirect
                         : ClientRedirectPolicy::kNotClientRedirect);
}
