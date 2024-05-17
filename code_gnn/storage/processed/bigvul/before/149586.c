void LoadingPredictorPreconnectTest::SetPreference() {
  profile_->GetPrefs()->SetInteger(
      prefs::kNetworkPredictionOptions,
      chrome_browser_net::NETWORK_PREDICTION_ALWAYS);
}
