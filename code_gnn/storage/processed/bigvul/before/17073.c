void OxideQQuickWebView::componentComplete() {
  Q_D(OxideQQuickWebView);

  Q_ASSERT(!d->constructed_);
  d->constructed_ = true;

  QQuickItem::componentComplete();

  OxideQQuickWebContext* context = nullptr;
  if (d->construct_props_->context) {
    context = d->construct_props_->context;
  }

  if (!context && !d->construct_props_->new_view_request) {
    context = OxideQQuickWebContext::defaultContext(true);
    if (!context) {
      qFatal("OxideQQuickWebView: No context available!");
    }
    OxideQQuickWebContextPrivate* cd =
        OxideQQuickWebContextPrivate::get(context);
    d->construct_props_->context = context;
    d->attachContextSignals(cd);
  }

  if (d->construct_props_->new_view_request ||
      OxideQQuickWebContextPrivate::get(context)->isConstructed()) {
    d->completeConstruction();
  }
}
