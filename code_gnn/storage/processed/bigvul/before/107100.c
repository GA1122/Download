void tst_QQuickWebView::loadEmptyPageViewHidden()
{
    QSignalSpy loadSpy(webView(), SIGNAL(loadingChanged(QWebLoadRequest*)));

    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForLoadSucceeded(webView()));

    QCOMPARE(loadSpy.size(), 2);
}
