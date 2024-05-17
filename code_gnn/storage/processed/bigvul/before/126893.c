BrowserView::~BrowserView() {
#if defined(USE_ASH)
  launcher_item_controller_.reset();
#endif

  preview_controller_.reset(NULL);

  browser_->tab_strip_model()->RemoveObserver(this);

#if defined(OS_WIN) && !defined(USE_AURA)
  ticker_.Stop();
  ticker_.UnregisterTickHandler(&hung_window_detector_);

  if (jumplist_) {
    jumplist_->Terminate();
  }
#endif

  download_shelf_.reset();

  if (tabstrip_) {
    tabstrip_->parent()->RemoveChildView(tabstrip_);
    delete tabstrip_;
    tabstrip_ = NULL;
  }
  RemoveAllChildViews(true);
  browser_.reset();
}
