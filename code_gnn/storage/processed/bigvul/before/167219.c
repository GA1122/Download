ExtensionDevToolsInfoBarDelegate::ExtensionDevToolsInfoBarDelegate(
    const base::Closure& dismissed_callback,
    const std::string& client_name)
    : ConfirmInfoBarDelegate(),
      client_name_(base::UTF8ToUTF16(client_name)),
      dismissed_callback_(dismissed_callback) {}
