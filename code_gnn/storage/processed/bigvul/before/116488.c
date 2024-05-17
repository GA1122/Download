ExtensionDevToolsInfoBarDelegate::ExtensionDevToolsInfoBarDelegate(
    InfoBarTabHelper* infobar_helper,
    const std::string& client_name,
    ExtensionDevToolsClientHost* client_host)
    : ConfirmInfoBarDelegate(infobar_helper),
      client_name_(client_name),
      client_host_(client_host) {
}
