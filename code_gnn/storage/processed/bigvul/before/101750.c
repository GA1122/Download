void Browser::OpenPasswordManager() {
  UserMetrics::RecordAction(UserMetricsAction("Options_ShowPasswordManager"));
  ShowOptionsTab(chrome::kPasswordManagerSubPage);
}
