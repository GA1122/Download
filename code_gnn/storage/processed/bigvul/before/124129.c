void ChromeContentBrowserClient::SetApplicationLocale(
    const std::string& locale) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
          base::Bind(
              &ChromeContentBrowserClient::SetApplicationLocaleOnIOThread,
              base::Unretained(this), locale)))
    io_thread_application_locale_ = locale;
}
