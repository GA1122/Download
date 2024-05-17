ExtensionDevToolsInfoBarDelegate::ExtensionDevToolsInfoBarDelegate(
    const std::string& client_name)
    : ConfirmInfoBarDelegate(),
      client_name_(client_name),
      client_host_(NULL) {
}
