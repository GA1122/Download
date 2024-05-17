void WebLocalFrameImpl::SetCanHaveScrollbars(bool can_have_scrollbars) {
  if (LocalFrameView* view = GetFrameView())
    view->SetCanHaveScrollbars(can_have_scrollbars);
}
