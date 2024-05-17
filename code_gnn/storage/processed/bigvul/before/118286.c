void AutofillDialogViews::OnWidgetDestroying(views::Widget* widget) {
  if (widget == window_)
    window_->GetRootView()->RemovePostTargetHandler(event_handler_.get());
}
