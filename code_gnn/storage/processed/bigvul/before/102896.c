void TabCloseableStateWatcher::OnBrowserRemoved(const Browser* browser) {
  if (browser->type() != Browser::TYPE_NORMAL)
    return;

  for (std::vector<TabStripWatcher*>::iterator it = tabstrip_watchers_.begin();
       it != tabstrip_watchers_.end(); ++it) {
    if ((*it)->browser() == browser) {
      delete (*it);
      tabstrip_watchers_.erase(it);
      break;
    }
  }

  CheckAndUpdateState(NULL);
}
