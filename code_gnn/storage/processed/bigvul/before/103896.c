void RenderView::didReceiveServerRedirectForProvisionalLoad(WebFrame* frame) {
  if (frame->parent())
    return;
  WebDataSource* data_source = frame->provisionalDataSource();
  if (!data_source) {
    NOTREACHED();
    return;
  }
  std::vector<GURL> redirects;
  GetRedirectChain(data_source, &redirects);
  if (redirects.size() >= 2) {
    bool has_opener_set = opener_id_ != MSG_ROUTING_NONE;
    Send(new ViewHostMsg_DidRedirectProvisionalLoad(routing_id_, page_id_,
        has_opener_set, redirects[redirects.size() - 2], redirects.back()));
  }
}
