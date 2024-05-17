void BrowserView::FocusAppMenu() {
  if (toolbar_->IsAppMenuFocused()) {
    RestoreFocus();
  } else {
    toolbar_->SetPaneFocusAndFocusAppMenu();
  }
}
