SingleDesktopTestObserver::SingleDesktopTestObserver(
    chrome::HostDesktopType allowed_desktop)
        : allowed_desktop_(allowed_desktop) {
  BrowserList::AddObserver(this);
}
