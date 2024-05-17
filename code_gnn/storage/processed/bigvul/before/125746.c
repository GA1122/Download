void TabSpecificContentSettings::GeolocationDidNavigate(
      const content::LoadCommittedDetails& details) {
  geolocation_settings_state_.DidNavigate(details);
}
