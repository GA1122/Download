gboolean BrowserWindowGtk::OnWindowState(GtkWidget* sender,
                                         GdkEventWindowState* event) {
  state_ = event->new_window_state;

  if (event->changed_mask & GDK_WINDOW_STATE_FULLSCREEN) {
    browser_->WindowFullscreenStateChanged();
    if (state_ & GDK_WINDOW_STATE_FULLSCREEN) {
      UpdateCustomFrame();
      toolbar_->Hide();
      tabstrip_->Hide();
      if (bookmark_bar_.get())
        gtk_widget_hide(bookmark_bar_->widget());
      bool is_kiosk =
          CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode);
      if (!is_kiosk && !fullscreen_exit_bubble_.get()) {
        fullscreen_exit_bubble_.reset(new FullscreenExitBubbleGtk(
            GTK_FLOATING_CONTAINER(render_area_floating_container_),
            browser(),
            GURL(),
            fullscreen_exit_bubble_type_));
      }
      gtk_widget_hide(titlebar_widget());
      gtk_widget_hide(toolbar_border_);
    } else {
      fullscreen_exit_bubble_type_ =
          FEB_TYPE_BROWSER_FULLSCREEN_EXIT_INSTRUCTION;
      gtk_widget_show(titlebar_widget());
      fullscreen_exit_bubble_.reset();
      UpdateCustomFrame();
      ShowSupportedWindowFeatures();
    }
  }

  if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) {
    content::NotificationService::current()->Notify(
        chrome::NOTIFICATION_BROWSER_WINDOW_MAXIMIZED,
        content::Source<BrowserWindow>(this),
        content::NotificationService::NoDetails());
  }

  titlebar_->UpdateCustomFrame(UseCustomFrame() && !IsFullscreen());
  UpdateWindowShape(bounds_.width(), bounds_.height());
  SaveWindowPosition();
  return FALSE;
}
