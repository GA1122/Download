void BrowserRenderProcessHost::ViewCreated() {
  visited_link_updater_->ReceiverReady(this);
}
