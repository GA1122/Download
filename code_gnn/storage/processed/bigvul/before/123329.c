  static gboolean OnMouseMoveEvent(GtkWidget* widget,
                                   GdkEventMotion* event,
                                   RenderWidgetHostViewGtk* host_view) {
    TRACE_EVENT0("browser",
                 "RenderWidgetHostViewGtkWidget::OnMouseMoveEvent");
    GtkWidget* event_widget = gtk_get_event_widget(
        reinterpret_cast<GdkEvent*>(event));
    if (event_widget != widget) {
      int x = 0;
      int y = 0;
      gtk_widget_get_pointer(widget, &x, &y);
      event->x = x;
      event->y = y;
    }

    host_view->ModifyEventForEdgeDragging(widget, event);

    WebKit::WebMouseEvent mouse_event =
        WebInputEventFactory::mouseEvent(event);

    if (host_view->mouse_locked_) {
      gfx::Point center = host_view->GetWidgetCenter();

      bool moved_to_center = MovedToPoint(mouse_event, center);
      if (moved_to_center)
        host_view->mouse_has_been_warped_to_new_center_ = true;

      host_view->ModifyEventMovementAndCoords(&mouse_event);

      if (!moved_to_center &&
          (mouse_event.movementX || mouse_event.movementY)) {
        GdkDisplay* display = gtk_widget_get_display(widget);
        GdkScreen* screen = gtk_widget_get_screen(widget);
        gdk_display_warp_pointer(display, screen, center.x(), center.y());
        if (host_view->mouse_has_been_warped_to_new_center_)
          RenderWidgetHostImpl::From(
              host_view->GetRenderWidgetHost())->ForwardMouseEvent(mouse_event);
      }
    } else {   
      host_view->ModifyEventMovementAndCoords(&mouse_event);
      if (!host_view->mouse_is_being_warped_to_unlocked_position_) {
        RenderWidgetHostImpl::From(
            host_view->GetRenderWidgetHost())->ForwardMouseEvent(mouse_event);
      }
    }
    return FALSE;
  }
