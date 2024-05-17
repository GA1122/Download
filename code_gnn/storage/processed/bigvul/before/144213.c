void LockContentsView::PerformAction(AcceleratorAction action) {
  switch (action) {
    case AcceleratorAction::kShowFeedback:
      Shell::Get()->login_screen_controller()->ShowFeedback();
      return;
    default:
      NOTREACHED();
  }
}
