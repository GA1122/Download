void BrowserActionButton::SetButtonNotPushed() {
  SetState(views::CustomButton::BS_NORMAL);
  menu_visible_ = false;
}
