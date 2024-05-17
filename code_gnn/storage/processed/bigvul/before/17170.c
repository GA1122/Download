void OxideQQuickWebView::setPopupMenu(QQmlComponent* popupMenu) {
  Q_D(OxideQQuickWebView);

  if (d->contents_view_->popupMenu() == popupMenu) {
    return;
  }

  d->contents_view_->setPopupMenu(popupMenu);
  emit popupMenuChanged();
}
