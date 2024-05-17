void ChromePasswordManagerClient::UserModifiedPasswordField() {
  if (GetMetricsRecorder()) {
    GetMetricsRecorder()->RecordUserModifiedPasswordField();
  }
}
