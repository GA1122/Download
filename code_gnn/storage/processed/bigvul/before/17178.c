void OxideQQuickWebView::setZoomFactor(qreal factor) {
  Q_D(OxideQQuickWebView);

  if (qFuzzyCompare(factor, zoomFactor())) {
    return;
  }

  if (!d->proxy_) {
    qWarning() <<
        "OxideQQuickWebView: zoom factor cannot be set during construction, "
        "it is a per-host value";
    return;
  }

  if (factor < minimumZoomFactor() || factor > maximumZoomFactor()) {
    qWarning() <<
        "OxideQQuickWebView: invalid value for zoom factor, expected to be "
        "between" << minimumZoomFactor() << "and" << maximumZoomFactor();
    return;
  }

  d->proxy_->setZoomFactor(factor);
}
