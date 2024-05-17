content::KeyboardEventProcessingResult Browser::PreHandleKeyboardEvent(
    content::WebContents* source,
    const NativeWebKeyboardEvent& event) {
  if (exclusive_access_manager_->HandleUserKeyEvent(event))
    return content::KeyboardEventProcessingResult::HANDLED;

  return window()->PreHandleKeyboardEvent(event);
}
