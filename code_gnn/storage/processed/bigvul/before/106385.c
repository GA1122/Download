void OfflineLoadPage::Observe(NotificationType type,
                              const NotificationSource& source,
                              const NotificationDetails& details) {
  if (type.value == NotificationType::NETWORK_STATE_CHANGED) {
    chromeos::NetworkStateDetails* state_details =
        Details<chromeos::NetworkStateDetails>(details).ptr();
    DVLOG(1) << "NetworkStateChanaged notification received: state="
             << state_details->state();
    if (state_details->state() ==
        chromeos::NetworkStateDetails::CONNECTED) {
      registrar_.Remove(this, NotificationType::NETWORK_STATE_CHANGED,
                        NotificationService::AllSources());
      Proceed();
    }
  } else {
    ChromeInterstitialPage::Observe(type, source, details);
  }
}
