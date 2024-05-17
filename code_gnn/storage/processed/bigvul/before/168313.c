bool BrowserView::IsToolbarVisible() const {
  if (immersive_mode_controller_->ShouldHideTopViews())
    return false;
  return (browser_->SupportsWindowFeature(Browser::FEATURE_TOOLBAR) ||
          browser_->SupportsWindowFeature(Browser::FEATURE_LOCATIONBAR)) &&
         toolbar_;
}
