ExtensionDevToolsClientHost::ExtensionDevToolsClientHost(
    WebContents* web_contents,
    const std::string& extension_id,
    const std::string& extension_name,
    int tab_id)
    : web_contents_(web_contents),
      extension_id_(extension_id),
      tab_id_(tab_id),
      last_request_id_(0),
      infobar_delegate_(NULL) {
  AttachedClientHosts::GetInstance()->Add(this);

  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNLOADED,
                 content::Source<Profile>(profile));

  DevToolsAgentHost* agent = DevToolsAgentHostRegistry::GetDevToolsAgentHost(
      web_contents_->GetRenderViewHost());
  DevToolsManager::GetInstance()->RegisterDevToolsClientHostFor(agent, this);

  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(web_contents_);
  infobar_delegate_ = new ExtensionDevToolsInfoBarDelegate(
      wrapper->infobar_tab_helper(), extension_name, this);
  wrapper->infobar_tab_helper()->AddInfoBar(infobar_delegate_);
}
