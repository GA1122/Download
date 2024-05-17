void BrowserActionsContainer::BubbleLostFocus(BrowserBubble* bubble,
                                              gfx::NativeView focused_view) {
  if (!popup_)
    return;

#if defined(OS_WIN)
  gfx::NativeView popup_native_view = popup_->native_view();
  gfx::NativeView parent = focused_view;
  while (parent = ::GetParent(parent)) {
    if (parent == popup_native_view)
      return;
  }
#endif

  MessageLoop::current()->PostTask(FROM_HERE,
      task_factory_.NewRunnableMethod(&BrowserActionsContainer::HidePopup));
}
