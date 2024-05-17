  static gboolean OnMouseScrollEvent(GtkWidget* widget,
                                     GdkEventScroll* event,
                                     RenderWidgetHostViewGtk* host_view) {
    TRACE_EVENT0("browser",
                 "RenderWidgetHostViewGtkWidget::OnMouseScrollEvent");
    if (event->state & GDK_SHIFT_MASK) {
      if (event->direction == GDK_SCROLL_UP)
        event->direction = GDK_SCROLL_LEFT;
      else if (event->direction == GDK_SCROLL_DOWN)
        event->direction = GDK_SCROLL_RIGHT;
    }

    WebMouseWheelEvent web_event = WebInputEventFactory::mouseWheelEvent(event);
    if (event->direction == GDK_SCROLL_UP ||
        event->direction == GDK_SCROLL_DOWN) {
      if (event->direction == GDK_SCROLL_UP)
        web_event.deltaY = kDefaultScrollPixelsPerTick;
      else
        web_event.deltaY = -kDefaultScrollPixelsPerTick;
      web_event.deltaY += GetPendingScrollDelta(true, event->state);
    } else {
      if (event->direction == GDK_SCROLL_LEFT)
        web_event.deltaX = kDefaultScrollPixelsPerTick;
      else
        web_event.deltaX = -kDefaultScrollPixelsPerTick;
      web_event.deltaX += GetPendingScrollDelta(false, event->state);
    }
    RenderWidgetHostImpl::From(
        host_view->GetRenderWidgetHost())->ForwardWheelEvent(web_event);
    return FALSE;
  }
