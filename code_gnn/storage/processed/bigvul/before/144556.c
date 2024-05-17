WebUI* WebContentsImpl::GetWebUI() const {
  WebUI* commited_web_ui = GetCommittedWebUI();
  return commited_web_ui ? commited_web_ui
                         : GetRenderManager()->GetNavigatingWebUI();
}
