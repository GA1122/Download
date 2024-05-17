const password_manager::LogManager* ChromePasswordManagerClient::GetLogManager()
    const {
  return log_manager_.get();
}
