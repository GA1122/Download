bool ChromeContentBrowserClient::IsShuttingDown() {
  return browser_shutdown::GetShutdownType() != browser_shutdown::NOT_VALID;
}
