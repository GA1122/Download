void RenderViewHostManager::SetWebUIPostCommit(WebUIImpl* web_ui) {
  DCHECK(!web_ui_.get());
  web_ui_.reset(web_ui);
}
