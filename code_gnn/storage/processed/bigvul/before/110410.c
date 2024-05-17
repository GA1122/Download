void WebUILoginView::Observe(int type,
                             const content::NotificationSource& source,
                             const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_LOGIN_WEBUI_VISIBLE: {
      OnLoginPromptVisible();
      registrar_.RemoveAll();
      break;
    }
    case content::NOTIFICATION_RENDER_VIEW_HOST_CREATED_FOR_TAB: {
      RenderViewHost* render_view_host =
          content::Details<RenderViewHost>(details).ptr();
      new SnifferObserver(render_view_host, GetWebUI());
      break;
    }
    default:
      NOTREACHED() << "Unexpected notification " << type;
  }
}
