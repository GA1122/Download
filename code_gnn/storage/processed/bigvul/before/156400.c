WebContentsImpl* PageHandler::GetWebContents() {
  return host_ && !host_->frame_tree_node()->parent()
             ? static_cast<WebContentsImpl*>(
                   WebContents::FromRenderFrameHost(host_))
             : nullptr;
}
