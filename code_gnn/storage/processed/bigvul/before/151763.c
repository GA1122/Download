void Browser::UpdateToolbar(bool should_restore_state) {
  window_->UpdateToolbar(should_restore_state ?
      tab_strip_model_->GetActiveWebContents() : NULL);
}
