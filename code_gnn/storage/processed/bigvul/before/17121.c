void OxideQQuickWebView::loadHtml(const QString& html, const QUrl& baseUrl) {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    d->construct_props_->load_html = true;
    d->construct_props_->html = html;
    d->construct_props_->url = baseUrl;
    return;
  }

  d->proxy_->loadHtml(html, baseUrl);
}
