WebUI* WebContentsImpl::GetCommittedWebUI() const {
  return GetRenderManager()->web_ui();
}
