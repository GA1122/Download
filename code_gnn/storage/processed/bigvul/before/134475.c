void HomePageUndoBubble::LinkClicked(views::Link* source, int event_flags) {
  PrefService* prefs = user_prefs::UserPrefs::Get(browser_->profile());
  prefs->SetBoolean(prefs::kHomePageIsNewTabPage, undo_value_is_ntp_);
  prefs->SetString(prefs::kHomePage, undo_url_.spec());

  HideBubble();
}
