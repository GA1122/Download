void TabSpecificContentSettings::DidNavigateMainFrame(
    const content::LoadCommittedDetails& details,
    const content::FrameNavigateParams& params) {
  if (!details.is_in_page) {
    ClearBlockedContentSettingsExceptForCookies();
    GeolocationDidNavigate(details);
  }
}
