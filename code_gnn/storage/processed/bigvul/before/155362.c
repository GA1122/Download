::rappor::RapporService* ChromeContentBrowserClient::GetRapporService() {
  return g_browser_process->rappor_service();
}
