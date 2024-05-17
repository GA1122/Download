ExtensionDevToolsClientHost::~ExtensionDevToolsClientHost() {
  if (infobar_delegate_) {
    infobar_delegate_->ClearClientHost();
    TabContentsWrapper* wrapper =
        TabContentsWrapper::GetCurrentWrapperForContents(web_contents_);
    InfoBarTabHelper* helper = wrapper->infobar_tab_helper();
    if (helper)
      helper->RemoveInfoBar(infobar_delegate_);
  }
  AttachedClientHosts::GetInstance()->Remove(this);
}
