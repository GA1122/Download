void Shell::CreateKeyboard() {
  if (in_mus_)
    return;
  InitKeyboard();
  GetPrimaryRootWindowController()->
      ActivateKeyboard(keyboard::KeyboardController::GetInstance());
}
