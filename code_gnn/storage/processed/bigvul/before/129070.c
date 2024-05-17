ExtensionDevToolsClientHost::ExtensionDevToolsClientHost(
    Profile* profile,
    DevToolsAgentHost* agent_host,
    const std::string& extension_id,
    const std::string& extension_name,
    const Debuggee& debuggee,
    infobars::InfoBar* infobar)
    : profile_(profile),
      agent_host_(agent_host),
      extension_id_(extension_id),
      last_request_id_(0),
      infobar_(infobar),
      detach_reason_(OnDetach::REASON_TARGET_CLOSED),
      extension_registry_observer_(this) {
  CopyDebuggee(&debuggee_, debuggee);

  AttachedClientHosts::GetInstance()->Add(this);

  extension_registry_observer_.Add(ExtensionRegistry::Get(profile_));

  registrar_.Add(this, chrome::NOTIFICATION_APP_TERMINATING,
                 content::NotificationService::AllSources());

  DevToolsManager::GetInstance()->RegisterDevToolsClientHostFor(
      agent_host_.get(), this);

  if (infobar_) {
    static_cast<ExtensionDevToolsInfoBarDelegate*>(
        infobar_->delegate())->set_client_host(this);
    registrar_.Add(
        this, chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_REMOVED,
        content::Source<InfoBarService>(InfoBarService::FromWebContents(
            WebContents::FromRenderViewHost(
                agent_host_->GetRenderViewHost()))));
  }
}
