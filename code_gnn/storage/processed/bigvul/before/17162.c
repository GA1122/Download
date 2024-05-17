void OxideQQuickWebView::setContextMenu(QQmlComponent* contextMenu) {
  Q_D(OxideQQuickWebView);

  if (d->contents_view_->contextMenu() == contextMenu) {
    return;
  }

  d->contents_view_->setContextMenu(contextMenu);
  emit contextMenuChanged();
}
