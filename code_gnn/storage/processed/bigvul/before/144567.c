void WebContentsImpl::InsertCSS(const std::string& css) {
  GetMainFrame()->Send(new FrameMsg_CSSInsertRequest(
      GetMainFrame()->GetRoutingID(), css));
}
