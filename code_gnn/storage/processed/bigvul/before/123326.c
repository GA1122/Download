  static gboolean OnFocusOut(GtkWidget* widget,
                             GdkEventFocus* focus,
                             RenderWidgetHostViewGtk* host_view) {
    gdk_window_set_cursor(gtk_widget_get_window(widget), NULL);
    if (!host_view->IsShowingContextMenu()) {
      RenderWidgetHostImpl* host =
          RenderWidgetHostImpl::From(host_view->GetRenderWidgetHost());
      host->SetActive(false);
      host->Blur();
    }

    host_view->was_imcontext_focused_before_grab_ = false;

    host_view->im_context_->OnFocusOut();

    host_view->set_last_mouse_down(NULL);

    return TRUE;
  }
