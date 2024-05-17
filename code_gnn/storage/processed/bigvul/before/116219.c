QQuickWebViewPrivate::QQuickWebViewPrivate(QQuickWebView* viewport)
    : q_ptr(viewport)
    , alertDialog(0)
    , confirmDialog(0)
    , promptDialog(0)
    , authenticationDialog(0)
    , certificateVerificationDialog(0)
    , itemSelector(0)
    , proxyAuthenticationDialog(0)
    , filePicker(0)
    , databaseQuotaDialog(0)
    , colorChooser(0)
    , m_useDefaultContentItemSize(true)
    , m_navigatorQtObjectEnabled(false)
    , m_renderToOffscreenBuffer(false)
    , m_dialogActive(false)
    , m_allowAnyHTTPSCertificateForLocalHost(false)
    , m_loadProgress(0)
{
    viewport->setClip(true);
    viewport->setPixelAligned(true);
    QObject::connect(viewport, SIGNAL(visibleChanged()), viewport, SLOT(_q_onVisibleChanged()));
    QObject::connect(viewport, SIGNAL(urlChanged()), viewport, SLOT(_q_onUrlChanged()));
    pageView.reset(new QQuickWebPage(viewport));
}
