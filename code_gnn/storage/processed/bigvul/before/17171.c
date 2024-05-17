void OxideQQuickWebView::setPreferences(OxideQWebPreferences* prefs) {
  Q_D(OxideQQuickWebView);

  if (prefs == this->preferences()) {
    return;
  }

  if (!d->proxy_) {
    d->construct_props_->preferences = prefs;
  } else {
    d->proxy_->setPreferences(prefs);
  }

  emit preferencesChanged();
}
