void OxideQQuickWebViewPrivate::RequestNotificationPermission(
    OxideQPermissionRequest* request) {
  Q_Q(OxideQQuickWebView);

  QQmlEngine* engine = qmlEngine(q);
  if (!engine) {
    delete request;
    return;
  }

  {
    QJSValue val = engine->newQObject(request);
    if (!val.isQObject()) {
      delete request;
      return;
    }

    emit q->notificationPermissionRequested(val);
  }

  engine->collectGarbage();
}
