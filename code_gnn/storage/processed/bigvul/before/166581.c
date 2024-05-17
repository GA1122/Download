void ToggleFullscreenToolbar(Browser* browser) {
  DCHECK(browser);

  PrefService* prefs = browser->profile()->GetPrefs();
  bool show_toolbar = prefs->GetBoolean(prefs::kShowFullscreenToolbar);
  prefs->SetBoolean(prefs::kShowFullscreenToolbar, !show_toolbar);
}
