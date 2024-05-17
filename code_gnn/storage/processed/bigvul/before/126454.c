bool BrowserWindowGtk::UseCustomFrame() const {
  return use_custom_frame_pref_.GetValue() && !browser_->is_app();
}
