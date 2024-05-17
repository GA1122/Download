views::View* ProfileChooserView::GetInitiallyFocusedView() {
#if defined(OS_MACOSX)
  if (!GetFocusManager() || !GetFocusManager()->keyboard_accessible())
    return nullptr;
#endif
  return signin_current_profile_button_;
}
