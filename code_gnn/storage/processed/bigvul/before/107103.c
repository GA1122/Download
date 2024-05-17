void tst_QQuickWebView::loadNonexistentFileUrl()
{
    QSignalSpy loadSpy(webView(), SIGNAL(loadingChanged(QWebLoadRequest*)));

    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/file_that_does_not_exist.html")));
    QVERIFY(waitForLoadFailed(webView()));

    QCOMPARE(loadSpy.size(), 2);
}
