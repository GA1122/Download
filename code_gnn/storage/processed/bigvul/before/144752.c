base::TimeDelta GetLongestObservationWindow() {
  const SiteCharacteristicsDatabaseParams& params =
      GetStaticSiteCharacteristicsDatabaseParams();
  return std::max({params.favicon_update_observation_window,
                   params.title_update_observation_window,
                   params.audio_usage_observation_window,
                   params.notifications_usage_observation_window});
}
