    void onLoadingChanged(QWebLoadRequest* loadRequest)
    {
        if (loadRequest->status() == QQuickWebView::LoadStartedStatus) {
            QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);

            QCOMPARE(m_webView->loading(), true);
        }
    }
