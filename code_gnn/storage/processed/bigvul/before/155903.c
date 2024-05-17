void ProfileChooserView::OnAvatarMenuChanged(
    AvatarMenu* avatar_menu) {
  if (IsProfileChooser(view_mode_) ||
      view_mode_ == profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT) {
    ShowView(view_mode_, avatar_menu);
  }
}
