bool BrowserWindowGtk::IsBookmarkBarVisible() const {
  return browser_->SupportsWindowFeature(Browser::FEATURE_BOOKMARKBAR) &&
         bookmark_bar_.get() &&
         browser_->profile()->GetPrefs()->GetBoolean(prefs::kShowBookmarkBar);
}
