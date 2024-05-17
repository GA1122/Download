void Browser::ContentRestrictionsChanged(WebContents* source) {
  command_controller_->ContentRestrictionsChanged();
}
