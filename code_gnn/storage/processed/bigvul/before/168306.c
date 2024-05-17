bool BrowserView::IsBookmarkBarVisible() const {
  if (!browser_->SupportsWindowFeature(Browser::FEATURE_BOOKMARKBAR))
    return false;
  if (!bookmark_bar_view_.get())
    return false;
  if (!bookmark_bar_view_->parent())
    return false;
  if (bookmark_bar_view_->GetPreferredSize().height() == 0)
    return false;
  if (immersive_mode_controller_->ShouldHideTopViews() &&
      !bookmark_bar_view_->IsDetached())
    return false;
  return true;
}
