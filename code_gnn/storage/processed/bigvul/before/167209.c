void GlobalConfirmInfoBar::OnInfoBarRemoved(infobars::InfoBar* info_bar,
                                            bool animate) {
  for (auto it : proxies_) {
    if (it.second->info_bar_ == info_bar) {
      OnManagerShuttingDown(info_bar->owner());
      break;
    }
  }
}
