void LauncherView::MouseExitedButton(views::View* view) {
  if (!tooltip_->IsVisible())
    tooltip_->StopTimer();
}
