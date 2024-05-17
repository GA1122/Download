void ToggleBookmarkBarWhenVisible(content::BrowserContext* browser_context) {
  PrefService* prefs = user_prefs::UserPrefs::Get(browser_context);
  const bool always_show =
      !prefs->GetBoolean(bookmarks::prefs::kShowBookmarkBar);

  prefs->SetBoolean(bookmarks::prefs::kShowBookmarkBar, always_show);
}
