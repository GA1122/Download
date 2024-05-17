WebUI* WebContentsImpl::GetWebUI() const {
  return GetRenderManager()->web_ui() ? GetRenderManager()->web_ui()
      : GetRenderManager()->pending_web_ui();
}
