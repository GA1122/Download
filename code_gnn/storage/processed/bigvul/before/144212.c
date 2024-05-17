void LockContentsView::OnStateChanged(
    const keyboard::KeyboardControllerState state) {
  if (!primary_big_view_)
    return;

  if (state == keyboard::KeyboardControllerState::SHOWN ||
      state == keyboard::KeyboardControllerState::HIDDEN) {
    bool keyboard_will_be_shown =
        state == keyboard::KeyboardControllerState::SHOWN;
    if (keyboard_shown_ == keyboard_will_be_shown)
      return;
    keyboard_shown_ = keyboard_will_be_shown;
    LayoutAuth(CurrentBigUserView(), nullptr  ,
               false  );
  }
}
