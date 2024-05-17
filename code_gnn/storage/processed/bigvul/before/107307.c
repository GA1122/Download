void Browser::OnContentSettingsChange(TabContents* source) {
  if (source == GetSelectedTabContents())
    window_->GetLocationBar()->UpdateContentSettingsIcons();
}
