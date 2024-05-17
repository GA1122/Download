void BrowserView::ShowAvatarBubbleFromAvatarButton(
    AvatarBubbleMode mode,
    const signin::ManageAccountsParams& manage_accounts_params,
    signin_metrics::AccessPoint access_point,
    bool focus_first_profile_button) {
#if !defined(OS_CHROMEOS)
  if (!IsRegularOrGuestSession())
    return;
  views::Button* avatar_button = toolbar_->avatar_button();
  if (!avatar_button)
    avatar_button = frame_->GetNewAvatarMenuButton();
  if (!avatar_button)
    return;

  profiles::BubbleViewMode bubble_view_mode;
  profiles::BubbleViewModeFromAvatarBubbleMode(mode, &bubble_view_mode);
  if (SigninViewController::ShouldShowSigninForMode(bubble_view_mode)) {
    browser_->signin_view_controller()->ShowSignin(
        bubble_view_mode, browser_.get(), access_point);
  } else {
    ProfileChooserView::ShowBubble(
        bubble_view_mode, manage_accounts_params, access_point, avatar_button,
        nullptr, gfx::Rect(), browser(), focus_first_profile_button);
    ProfileMetrics::LogProfileOpenMethod(ProfileMetrics::ICON_AVATAR_BUBBLE);
  }
#else
  NOTREACHED();
#endif
}
