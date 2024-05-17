void BrowserView::UpdateReloadStopState(bool is_loading, bool force) {
  if (toolbar_->reload_button()) {
    toolbar_->reload_button()->ChangeMode(
        is_loading ? ReloadButton::Mode::kStop : ReloadButton::Mode::kReload,
        force);
  }
}
