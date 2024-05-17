UserSelectionScreen::GetScreenType() const {
  if (display_type_ == OobeUI::kLockDisplay)
    return LOCK_SCREEN;

  if (display_type_ == OobeUI::kLoginDisplay)
    return SIGNIN_SCREEN;

  return OTHER_SCREEN;
}
