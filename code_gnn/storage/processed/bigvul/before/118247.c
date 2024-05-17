bool AutofillDialogViews::HandleKeyEvent(views::Textfield* sender,
                                         const ui::KeyEvent& key_event) {
  content::NativeWebKeyboardEvent event(key_event);
  return delegate_->HandleKeyPressEventInInput(event);
}
