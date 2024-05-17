bool WebLocalFrameImpl::IsFocused() const {
  if (!ViewImpl() || !ViewImpl()->GetPage())
    return false;

  return this ==
         WebFrame::FromFrame(
             ViewImpl()->GetPage()->GetFocusController().FocusedFrame());
}
