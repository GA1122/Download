keyboard::KeyboardController* LockContentsView::GetKeyboardController() const {
  return GetWidget() ? GetKeyboardControllerForWidget(GetWidget()) : nullptr;
}
