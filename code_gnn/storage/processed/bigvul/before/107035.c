void QQuickWebViewFlickablePrivate::onComponentComplete()
{
    Q_Q(QQuickWebView);

    ASSERT(!flickProvider);
    flickProvider = new QtFlickProvider(q, pageView.data());

    const QQuickWebViewExperimental* experimental = q->experimental();
    QObject::connect(flickProvider, SIGNAL(contentWidthChanged()), experimental, SIGNAL(contentWidthChanged()));
    QObject::connect(flickProvider, SIGNAL(contentHeightChanged()), experimental, SIGNAL(contentHeightChanged()));
    QObject::connect(flickProvider, SIGNAL(contentXChanged()), experimental, SIGNAL(contentXChanged()));
    QObject::connect(flickProvider, SIGNAL(contentYChanged()), experimental, SIGNAL(contentYChanged()));

    interactionEngine.reset(new QtViewportInteractionEngine(q, pageView.data(), flickProvider));
    pageView->eventHandler()->setViewportInteractionEngine(interactionEngine.data());

    QObject::connect(interactionEngine.data(), SIGNAL(contentSuspendRequested()), q, SLOT(_q_suspend()));
    QObject::connect(interactionEngine.data(), SIGNAL(contentResumeRequested()), q, SLOT(_q_resume()));
    QObject::connect(interactionEngine.data(), SIGNAL(contentWasMoved(const QPointF&)), q, SLOT(_q_commitPositionChange(const QPointF&)));
    QObject::connect(interactionEngine.data(), SIGNAL(contentWasScaled()), q, SLOT(_q_commitScaleChange()));

    _q_resume();

    if (loadSuccessDispatchIsPending) {
        QQuickWebViewPrivate::loadDidSucceed();
        loadSuccessDispatchIsPending = false;
    }

    _q_onVisibleChanged();

    QQuickWebViewPrivate::onComponentComplete();
}
