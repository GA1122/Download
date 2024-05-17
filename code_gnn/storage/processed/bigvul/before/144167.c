void LockContentsView::AddRotationAction(const OnRotate& on_rotate) {
  on_rotate.Run(login_layout_util::ShouldShowLandscape(GetWidget()));
  rotation_actions_.push_back(on_rotate);
}
