  static gboolean OnClientEvent(GtkWidget* widget,
                                GdkEventClient* event,
                                RenderWidgetHostViewGtk* host_view) {
    VLOG(1) << "client event type: " << event->message_type
            << " data_format: " << event->data_format
            << " data: " << event->data.l;
    return TRUE;
  }
