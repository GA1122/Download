void BrowserWindowGtk::ShowAvatarBubbleFromAvatarButton() {
  if (titlebar_->avatar_button())
    titlebar_->avatar_button()->ShowAvatarBubble();
}
