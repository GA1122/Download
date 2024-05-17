void WebLocalFrameImpl::ReloadWithOverrideURL(const WebURL& override_url,
                                              WebFrameLoadType load_type) {
  DCHECK(GetFrame());
  DCHECK(IsReloadLoadType(static_cast<FrameLoadType>(load_type)));
  WebURLRequest request = RequestForReload(load_type, override_url);
  if (request.IsNull())
    return;
  Load(request, load_type, WebHistoryItem(), kWebHistoryDifferentDocumentLoad,
       false);
}
