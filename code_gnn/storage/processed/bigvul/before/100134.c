void BrowserActionButton::SetButtonPushed() {
  SetState(views::CustomButton::BS_PUSHED);
  menu_visible_ = true;
}
