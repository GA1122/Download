void WebContentsImpl::StopFinding(StopFindAction action) {
  if (browser_plugin_embedder_ &&
      browser_plugin_embedder_->StopFinding(action)) {
    return;
  }
  GetMainFrame()->Send(
      new FrameMsg_StopFinding(GetMainFrame()->GetRoutingID(), action));
}
