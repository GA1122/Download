void ConfirmInfoBar::ButtonPressed(
    views::Button* sender, const views::Event& event) {
  InfoBar::ButtonPressed(sender, event);
  if (sender == ok_button_) {
    if (GetDelegate()->Accept())
      RemoveInfoBar();
  } else if (sender == cancel_button_) {
    if (GetDelegate()->Cancel())
      RemoveInfoBar();
  }
}
