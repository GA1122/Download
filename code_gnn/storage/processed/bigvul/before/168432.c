TestBrowserWindowOwner::TestBrowserWindowOwner(TestBrowserWindow* window)
    : window_(window) {
  BrowserList::AddObserver(this);
}
