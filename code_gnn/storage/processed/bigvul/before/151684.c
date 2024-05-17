base::string16 Browser::GetWindowTitleForCurrentTab(
    bool include_app_name) const {
  return GetWindowTitleFromWebContents(
      include_app_name, tab_strip_model_->GetActiveWebContents());
}
