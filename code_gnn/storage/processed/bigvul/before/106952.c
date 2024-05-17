QQuickWebViewPrivate::QQuickWebViewPrivate(QQuickWebView* viewport)
    : q_ptr(viewport)
    , flickProvider(0)
    , alertDialog(0)
    , confirmDialog(0)
    , promptDialog(0)
    , authenticationDialog(0)
    , certificateVerificationDialog(0)
    , itemSelector(0)
    , proxyAuthenticationDialog(0)
    , userDidOverrideContentWidth(false)
    , userDidOverrideContentHeight(false)
    , m_navigatorQtObjectEnabled(false)
    , m_renderToOffscreenBuffer(false)
    , m_loadStartedSignalSent(false)
    , m_dialogRunnerActive(false)
{
    viewport->setFlags(QQuickItem::ItemClipsChildrenToShape);
    QObject::connect(viewport, SIGNAL(visibleChanged()), viewport, SLOT(_q_onVisibleChanged()));
    QObject::connect(viewport, SIGNAL(urlChanged()), viewport, SLOT(_q_onUrlChanged()));
    pageView.reset(new QQuickWebPage(viewport));
}
