void BeforeTranslateInfoBar::ButtonPressed(views::Button* sender,
                                           const views::Event& event) {
  if (sender == accept_button_) {
    GetDelegate()->Translate();
  } else if (sender == deny_button_) {
    RemoveInfoBar();
    GetDelegate()->TranslationDeclined();
  } else if (sender == never_translate_button_) {
    GetDelegate()->NeverTranslatePageLanguage();
  } else if (sender == always_translate_button_) {
    GetDelegate()->AlwaysTranslatePageLanguage();
  } else {
    TranslateInfoBarBase::ButtonPressed(sender, event);
  }
}
