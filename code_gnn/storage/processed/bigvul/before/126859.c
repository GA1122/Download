void BrowserView::ShowAvatarBubbleFromAvatarButton() {
  AvatarMenuButton* button = frame_->GetAvatarMenuButton();
  if (button)
    button->ShowAvatarBubble();
}
