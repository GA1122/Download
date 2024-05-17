void BrowserChildProcessHostImpl::RemoveObserver(
    BrowserChildProcessObserver* observer) {
  g_browser_child_process_observers.Get().RemoveObserver(observer);
}
