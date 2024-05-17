void LoginHtmlDialog::Show() {
  HtmlDialogWithoutContextMenuView* html_view =
      new HtmlDialogWithoutContextMenuView(ProfileManager::GetDefaultProfile(),
                                           this);
  if (style_ & STYLE_BUBBLE) {
    views::Window* bubble_window = BubbleWindow::Create(
        parent_window_, gfx::Rect(),
        static_cast<BubbleWindow::Style>(
            BubbleWindow::STYLE_XBAR | BubbleWindow::STYLE_THROBBER),
        html_view);
    bubble_frame_view_ = static_cast<BubbleFrameView*>(
        bubble_window->non_client_view()->frame_view());
  } else {
    views::Window::CreateChromeWindow(parent_window_, gfx::Rect(), html_view);
  }
  if (bubble_frame_view_) {
    bubble_frame_view_->StartThrobber();
    notification_registrar_.Add(this,
                                NotificationType::LOAD_COMPLETED_MAIN_FRAME,
                                NotificationService::AllSources());
  }
  html_view->InitDialog();
  html_view->window()->Show();
  is_open_ = true;
}
