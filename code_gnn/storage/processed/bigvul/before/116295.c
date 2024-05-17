void QQuickWebView::setAllowAnyHTTPSCertificateForLocalHost(bool allow)
{
    Q_D(QQuickWebView);
    d->m_allowAnyHTTPSCertificateForLocalHost = allow;
}
