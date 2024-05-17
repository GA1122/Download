ExtensionDevToolsInfoBarDelegate::~ExtensionDevToolsInfoBarDelegate() {
  if (client_host_)
      client_host_->InfoBarDestroyed();
}
