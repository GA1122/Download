void BrowserWindowGtk::UpdateReloadStopState(bool is_loading, bool force) {
  toolbar_->GetReloadButton()->ChangeMode(
      is_loading ? ReloadButtonGtk::MODE_STOP : ReloadButtonGtk::MODE_RELOAD,
      force);
}
