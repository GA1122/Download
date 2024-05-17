BrowserEventRouter::~BrowserEventRouter() {
  BrowserList::RemoveObserver(this);
}
