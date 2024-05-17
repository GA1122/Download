void LockContentsView::ShowAuthErrorMessage() {
  LoginBigUserView* big_view = CurrentBigUserView();
  if (!big_view->auth_user())
    return;

  if (screen_type_ == LockScreen::ScreenType::kLogin &&
      unlock_attempt_ >= kLoginAttemptsBeforeGaiaDialog) {
    Shell::Get()->login_screen_controller()->ShowGaiaSignin(
        true  ,
        big_view->auth_user()->current_user()->basic_user_info->account_id);
    return;
  }

  base::string16 error_text = l10n_util::GetStringUTF16(
      unlock_attempt_ > 1 ? IDS_ASH_LOGIN_ERROR_AUTHENTICATING_2ND_TIME
                          : IDS_ASH_LOGIN_ERROR_AUTHENTICATING);
  ImeController* ime_controller = Shell::Get()->ime_controller();
  if (ime_controller->IsCapsLockEnabled()) {
    error_text += base::ASCIIToUTF16(" ") +
                  l10n_util::GetStringUTF16(IDS_ASH_LOGIN_ERROR_CAPS_LOCK_HINT);
  }

  base::Optional<int> bold_start;
  int bold_length = 0;
  if (ime_controller->available_imes().size() > 1) {
    error_text += base::ASCIIToUTF16(" ");
    bold_start = error_text.length();
    base::string16 shortcut =
        l10n_util::GetStringUTF16(IDS_ASH_LOGIN_KEYBOARD_SWITCH_SHORTCUT);
    bold_length = shortcut.length();

    size_t shortcut_offset_in_string;
    error_text +=
        l10n_util::GetStringFUTF16(IDS_ASH_LOGIN_ERROR_KEYBOARD_SWITCH_HINT,
                                   shortcut, &shortcut_offset_in_string);
    *bold_start += shortcut_offset_in_string;
  }

  views::StyledLabel* label = new views::StyledLabel(error_text, this);
  MakeSectionBold(label, error_text, bold_start, bold_length);
  label->set_auto_color_readability_enabled(false);

  auth_error_bubble_->ShowErrorBubble(
      label, big_view->auth_user()->password_view()  ,
      LoginBubble::kFlagsNone);
}
