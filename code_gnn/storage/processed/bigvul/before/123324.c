  static gboolean OnExposeEvent(GtkWidget* widget,
                                GdkEventExpose* expose,
                                RenderWidgetHostViewGtk* host_view) {
    if (host_view->is_hidden_)
      return FALSE;
    const gfx::Rect damage_rect(expose->area);
    host_view->Paint(damage_rect);
    return FALSE;
  }
