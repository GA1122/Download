void Editor::ToggleOverwriteModeEnabled() {
  overwrite_mode_enabled_ = !overwrite_mode_enabled_;
  GetFrame().Selection().SetShouldShowBlockCursor(overwrite_mode_enabled_);
}
