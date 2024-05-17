void Browser::ShowContentSettingsWindow(ContentSettingsType content_type) {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(
        chrome::kContentSettingsSubPage + std::string(kHashMark) +
        ContentSettingsHandler::ContentSettingsTypeToGroupName(content_type));
  } else {
    window()->ShowContentSettingsWindow(content_type,
                                        profile_->GetOriginalProfile());
  }
}
