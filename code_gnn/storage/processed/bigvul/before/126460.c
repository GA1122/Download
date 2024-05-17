BrowserWindowGtk::~BrowserWindowGtk() {
  ui::ActiveWindowWatcherX::RemoveObserver(this);

  browser_->tab_strip_model()->RemoveObserver(this);
}
