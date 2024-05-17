  void ShowKeyboard(bool show) {
    keyboard::KeyboardController* keyboard =
        keyboard::KeyboardController::GetInstance();
    ASSERT_TRUE(keyboard);
    if (show == keyboard->keyboard_visible())
      return;

    if (show) {
      keyboard->ShowKeyboard(true);
      if (keyboard->ui()->GetContentsWindow()->bounds().height() == 0) {
        keyboard->ui()->GetContentsWindow()->SetBounds(
            keyboard::KeyboardBoundsFromRootBounds(
                Shell::GetPrimaryRootWindow()->bounds(), 100));
      }
    } else {
      keyboard->HideKeyboard(keyboard::KeyboardController::HIDE_REASON_MANUAL);
    }

    DCHECK_EQ(show, keyboard->keyboard_visible());
  }
