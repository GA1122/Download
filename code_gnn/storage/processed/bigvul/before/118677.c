void SingleDesktopTestObserver::OnBrowserAdded(Browser* browser) {
  CHECK(CalledOnValidThread());
  CHECK_EQ(browser->host_desktop_type(), allowed_desktop_);
}
