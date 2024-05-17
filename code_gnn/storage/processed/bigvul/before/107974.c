void TranslateMessageInfoBar::ButtonPressed(views::Button* sender,
                                            const views::Event& event) {
  if (sender == button_) {
    GetDelegate()->MessageInfoBarButtonPressed();
    return;
  }
  TranslateInfoBarBase::ButtonPressed(sender, event);
}
