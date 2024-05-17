TabCloseableStateWatcher::TabStripWatcher::~TabStripWatcher() {
  browser_->tabstrip_model()->RemoveObserver(this);
}
