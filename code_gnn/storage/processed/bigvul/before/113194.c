void LauncherView::MouseEnteredButton(views::View* view) {
  if (tooltip_->IsVisible()) {
    tooltip_->Close();
    tooltip_->ShowImmediately(view, GetAccessibleName(view));
  } else {
    tooltip_->ShowDelayed(view, GetAccessibleName(view));
  }
}
