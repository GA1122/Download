bool ShouldShowAppsShortcutInBookmarkBar(Profile* profile) {
  return IsAppsShortcutEnabled(profile) &&
         profile->GetPrefs()->GetBoolean(
             bookmarks::prefs::kShowAppsShortcutInBookmarkBar);
}
