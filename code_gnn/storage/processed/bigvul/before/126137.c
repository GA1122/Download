void BrowserLauncherItemController::Clicked() {
  views::Widget* widget =
      views::Widget::GetWidgetForNativeView(window_);
  if (widget && widget->IsActive()) {
    widget->Minimize();
  } else {
    Activate();
  }
}
