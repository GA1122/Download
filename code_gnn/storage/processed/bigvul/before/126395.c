gboolean BrowserWindowGtk::OnButtonPressEvent(GtkWidget* widget,
                                              GdkEventButton* event) {
  if (event->type == GDK_BUTTON_PRESS) {
    if (event->button == 8) {
      chrome::GoBack(browser_.get(), CURRENT_TAB);
      return TRUE;
    } else if (event->button == 9) {
      chrome::GoForward(browser_.get(), CURRENT_TAB);
      return TRUE;
    }
  }


  int win_x, win_y;
  GdkWindow* gdk_window = gtk_widget_get_window(GTK_WIDGET(window_));
  gdk_window_get_origin(gdk_window, &win_x, &win_y);

  GdkWindowEdge edge;
  gfx::Point point(static_cast<int>(event->x_root - win_x),
                   static_cast<int>(event->y_root - win_y));
  bool has_hit_edge = GetWindowEdge(point.x(), point.y(), &edge);

  GtkWidget* toolbar = toolbar_->widget();
  if (!gtk_widget_get_visible(toolbar)) {
    toolbar = render_area_vbox_;
  }
  gint toolbar_y;
  gtk_widget_get_pointer(toolbar, NULL, &toolbar_y);
  bool has_hit_titlebar = !IsFullscreen() && (toolbar_y < 0)
                          && !has_hit_edge;
  if (event->button == 1) {
    if (GDK_BUTTON_PRESS == event->type) {
      if ((has_hit_titlebar || has_hit_edge) && !suppress_window_raise_)
        gdk_window_raise(gdk_window);

      if (has_hit_titlebar) {
        return gtk_window_util::HandleTitleBarLeftMousePress(
            window_, bounds_, event);
      } else if (has_hit_edge) {
        gtk_window_begin_resize_drag(window_, edge, event->button,
                                     static_cast<gint>(event->x_root),
                                     static_cast<gint>(event->y_root),
                                     event->time);
        return TRUE;
      }
    } else if (GDK_2BUTTON_PRESS == event->type) {
      if (has_hit_titlebar) {
        if (IsMaximized()) {
          UnMaximize();
        } else {
          gtk_window_maximize(window_);
        }
        return TRUE;
      }
    }
  } else if (event->button == 2) {
    if (has_hit_titlebar || has_hit_edge) {
      gdk_window_lower(gdk_window);
    }
    return TRUE;
  } else if (event->button == 3) {
    if (has_hit_titlebar) {
      titlebar_->ShowContextMenu(event);
      return TRUE;
    }
  }

  return FALSE;   
}
