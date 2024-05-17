void AfterTranslateInfoBar::ButtonPressed(views::Button* sender,
                                          const views::Event& event) {
  if (sender == revert_button_) {
    GetDelegate()->RevertTranslation();
    return;
  }
  TranslateInfoBarBase::ButtonPressed(sender, event);
}
