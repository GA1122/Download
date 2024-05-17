ui::Clipboard* ChromeContentBrowserClient::GetClipboard() {
  return g_browser_process->clipboard();
}
