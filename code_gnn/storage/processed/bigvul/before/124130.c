void ChromeContentBrowserClient::SetApplicationLocaleOnIOThread(
    const std::string& locale) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  io_thread_application_locale_ = locale;
}
