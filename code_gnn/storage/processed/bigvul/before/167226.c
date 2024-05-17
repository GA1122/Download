ExtensionDevToolsInfoBar::~ExtensionDevToolsInfoBar() {
  g_extension_info_bars.Get().erase(extension_id_);
  if (infobar_)
    infobar_->Close();
}
