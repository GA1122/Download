void ChromePasswordManagerClient::RecordSavePasswordProgress(
    const std::string& log) {
  GetLogManager()->LogSavePasswordProgress(log);
}
