void Browser::OpenPasswordManager() {
  UserMetrics::RecordAction(UserMetricsAction("Options_ShowPasswordManager"),
                            profile_);
  ShowOptionsTab(chrome::kPasswordManagerSubPage);
}
