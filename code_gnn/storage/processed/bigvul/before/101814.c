void Browser::ShowContentSettingsPage(ContentSettingsType content_type) {
  ShowOptionsTab(
      chrome::kContentSettingsExceptionsSubPage + std::string(kHashMark) +
      ContentSettingsHandler::ContentSettingsTypeToGroupName(content_type));
}
