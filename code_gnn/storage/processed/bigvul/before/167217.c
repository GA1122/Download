ExtensionDevToolsInfoBar* ExtensionDevToolsInfoBar::Create(
    const std::string& extension_id,
    const std::string& extension_name,
    ExtensionDevToolsClientHost* client_host,
    const base::Closure& dismissed_callback) {
  ExtensionInfoBars::iterator it =
      g_extension_info_bars.Get().find(extension_id);
  ExtensionDevToolsInfoBar* infobar = nullptr;
  if (it != g_extension_info_bars.Get().end())
    infobar = it->second;
  else
    infobar = new ExtensionDevToolsInfoBar(extension_id, extension_name);
  infobar->callbacks_[client_host] = dismissed_callback;
  return infobar;
}
