std::string ChromeContentBrowserClient::GetApplicationLocale() {
  if (BrowserThread::CurrentlyOn(BrowserThread::IO))
    return io_thread_application_locale_;
  return g_browser_process->GetApplicationLocale();
}
