void ChromeInvalidationClient::UpdateCredentials(
    const std::string& email, const std::string& token) {
  DCHECK(CalledOnValidThread());
   chrome_system_resources_.network()->UpdateCredentials(email, token);
 }
