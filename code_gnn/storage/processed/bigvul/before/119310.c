void TranslateInfoBarDelegate::ToggleTranslatableLanguageByPrefs() {
  if (ui_delegate_.IsLanguageBlocked()) {
    ui_delegate_.SetLanguageBlocked(false);
  } else {
    ui_delegate_.SetLanguageBlocked(true);
    infobar()->RemoveSelf();
  }
}
