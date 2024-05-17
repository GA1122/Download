LRESULT InputMethodIMM32::OnImeNotify(UINT message,
                                      WPARAM wparam,
                                      LPARAM lparam,
                                      BOOL* handled) {
  *handled = FALSE;

  switch (wparam) {
  case IMN_OPENCANDIDATE:
    is_candidate_popup_open_ = true;
    break;
  case IMN_CLOSECANDIDATE:
    is_candidate_popup_open_ = false;
    break;
  }

  return 0;
}
