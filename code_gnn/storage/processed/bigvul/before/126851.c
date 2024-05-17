void BrowserView::SetWindowSwitcherButton(views::Button* button) {
  if (window_switcher_button_)
    RemoveChildView(window_switcher_button_);
  window_switcher_button_ = button;
  AddChildView(button);
}
