void CloudPolicySubsystem::Observe(int type,
                                   const NotificationSource& source,
                                   const NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_PREF_CHANGED) {
    DCHECK_EQ(*(Details<std::string>(details).ptr()),
              std::string(refresh_pref_name_));
    PrefService* local_state = g_browser_process->local_state();
    DCHECK_EQ(Source<PrefService>(source).ptr(), local_state);
    UpdatePolicyRefreshRate(local_state->GetInteger(refresh_pref_name_));
  } else {
    NOTREACHED();
  }
}
