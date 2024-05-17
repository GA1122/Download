void ThumbnailGenerator::Observe(NotificationType type,
                                 const NotificationSource& source,
                                 const NotificationDetails& details) {
  switch (type.value) {
    case NotificationType::RENDER_VIEW_HOST_CREATED_FOR_TAB: {
      RenderViewHost* renderer = Details<RenderViewHost>(details).ptr();
      renderer->set_painting_observer(this);
      break;
    }

    case NotificationType::RENDER_WIDGET_VISIBILITY_CHANGED:
      if (*Details<bool>(details).ptr())
        WidgetShown(Source<RenderWidgetHost>(source).ptr());
      else
        WidgetHidden(Source<RenderWidgetHost>(source).ptr());
      break;

    case NotificationType::RENDER_WIDGET_HOST_DESTROYED:
      WidgetDestroyed(Source<RenderWidgetHost>(source).ptr());
      break;

    case NotificationType::TAB_CONTENTS_DISCONNECTED:
      TabContentsDisconnected(Source<TabContents>(source).ptr());
      break;

    default:
      NOTREACHED();
  }
}
