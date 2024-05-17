 void FrameSelection::SetFrameIsFocused(bool flag) {
   if (focused_ == flag)
     return;
  focused_ = flag;

  FocusedOrActiveStateChanged();
}
