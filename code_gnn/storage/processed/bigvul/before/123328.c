  static gboolean OnKeyPressReleaseEvent(GtkWidget* widget,
                                         GdkEventKey* event,
                                         RenderWidgetHostViewGtk* host_view) {
    TRACE_EVENT0("browser",
                 "RenderWidgetHostViewGtkWidget::OnKeyPressReleaseEvent");
    bool should_close_on_escape =
        (host_view->IsPopup() && host_view->NeedsInputGrab()) ||
        host_view->is_fullscreen_;
    if (should_close_on_escape && GDK_Escape == event->keyval) {
      host_view->host_->Shutdown();
    } else if (host_view->host_ &&
               host_view->host_->KeyPressListenersHandleEvent(event)) {
      return TRUE;
    } else {
      host_view->im_context_->ProcessKeyEvent(event);
    }

    return TRUE;
  }
