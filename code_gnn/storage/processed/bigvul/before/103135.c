void Browser::OnContentSettingsChange(TabContents* source) {
  if (source == GetSelectedTabContents()) {
    LocationBar* location_bar = window()->GetLocationBar();
    if (location_bar)
      location_bar->UpdateContentSettingsIcons();
  }
}
