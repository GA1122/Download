  static gboolean OnFocusIn(GtkWidget* widget,
                            GdkEventFocus* focus,
                            RenderWidgetHostViewGtk* host_view) {
    host_view->ShowCurrentCursor();
    RenderWidgetHostImpl* host =
        RenderWidgetHostImpl::From(host_view->GetRenderWidgetHost());
    host->GotFocus();
    host->SetActive(true);

    host_view->im_context_->OnFocusIn();

    return TRUE;
  }
