void LauncherView::MouseMovedOverButton(views::View* view) {
  if (!tooltip_->IsVisible())
    tooltip_->ResetTimer();
}
