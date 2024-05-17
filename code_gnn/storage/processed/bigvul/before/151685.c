base::string16 Browser::GetWindowTitleForTab(bool include_app_name,
                                             int index) const {
  return GetWindowTitleFromWebContents(
      include_app_name, tab_strip_model_->GetWebContentsAt(index));
}
