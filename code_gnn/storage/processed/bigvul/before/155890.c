 int ProfileChooserView::GetDiceSigninPromoShowCount() const {
  return browser_->profile()->GetPrefs()->GetInteger(
      prefs::kDiceSigninUserMenuPromoCount);
}
