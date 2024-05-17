TestingAutomationProvider::~TestingAutomationProvider() {
#if defined(OS_CHROMEOS)
  RemoveChromeosObservers();
#endif
  BrowserList::RemoveObserver(this);
}
