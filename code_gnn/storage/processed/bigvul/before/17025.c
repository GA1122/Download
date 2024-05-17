void OxideQQuickWebViewPrivate::CertificateError(
    std::unique_ptr<OxideQCertificateError> cert_error) {
  Q_Q(OxideQQuickWebView);

  QQmlEngine* engine = qmlEngine(q);
  if (!engine) {
    return;
  }

  {
    QJSValue val = engine->newQObject(cert_error.get());
    if (!val.isQObject()) {
      return;
    }

    cert_error.release();

    emit q->certificateError(val);
  }

  engine->collectGarbage();
}
