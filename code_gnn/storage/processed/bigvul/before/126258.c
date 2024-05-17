void Browser::UpdateToolbar(bool should_restore_state) {
  window_->UpdateToolbar(chrome::GetActiveTabContents(this),
                         should_restore_state);
}
