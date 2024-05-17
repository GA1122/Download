ExtensionDevToolsInfoBar::ExtensionDevToolsInfoBar(
    const std::string& extension_id,
    const std::string& extension_name)
    : extension_id_(extension_id) {
  g_extension_info_bars.Get()[extension_id] = this;

  auto delegate = std::make_unique<ExtensionDevToolsInfoBarDelegate>(
      base::Bind(&ExtensionDevToolsInfoBar::InfoBarDismissed,
                 base::Unretained(this)),
      extension_name);
  infobar_ = GlobalConfirmInfoBar::Show(std::move(delegate));
}
