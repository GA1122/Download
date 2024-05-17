ExtensionDevToolsClientHost::~ExtensionDevToolsClientHost() {
  registrar_.RemoveAll();

  if (infobar_) {
    static_cast<ExtensionDevToolsInfoBarDelegate*>(
        infobar_->delegate())->set_client_host(NULL);
    InfoBarService* infobar_service = InfoBarService::FromWebContents(
        WebContents::FromRenderViewHost(agent_host_->GetRenderViewHost()));
    infobar_service->RemoveInfoBar(infobar_);
  }
  AttachedClientHosts::GetInstance()->Remove(this);
}
