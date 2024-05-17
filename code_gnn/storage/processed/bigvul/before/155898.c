void ProfileChooserView::IncrementDiceSigninPromoShowCount() {
  browser_->profile()->GetPrefs()->SetInteger(
      prefs::kDiceSigninUserMenuPromoCount, GetDiceSigninPromoShowCount() + 1);
}
