void AutofillDialogViews::Show() {
  InitChildViews();
  UpdateAccountChooser();
  UpdateNotificationArea();
  UpdateButtonStripExtraView();

  window_ = constrained_window::ShowWebModalDialogViews(
      this, delegate_->GetWebContents());
  focus_manager_ = window_->GetFocusManager();
  focus_manager_->AddFocusChangeListener(this);

  ShowDialogInMode(DETAIL_INPUT);

  views::Widget* browser_widget =
      views::Widget::GetTopLevelWidgetForNativeView(
          delegate_->GetWebContents()->GetNativeView());
  observer_.Add(browser_widget);

  event_handler_.reset(new MousePressedHandler(delegate_));
  window_->GetRootView()->AddPostTargetHandler(event_handler_.get());
  observer_.Add(window_);
}
