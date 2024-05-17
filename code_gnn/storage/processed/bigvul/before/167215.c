GlobalConfirmInfoBar::~GlobalConfirmInfoBar() {
  while (!proxies_.empty()) {
    auto it = proxies_.begin();
    it->second->Detach();
    it->first->RemoveObserver(this);
    it->first->RemoveInfoBar(it->second->info_bar_);
    proxies_.erase(it);
  }
}
