void tst_QQuickWebView::loadProgress()
{
    QCOMPARE(webView()->loadProgress(), 0);

    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QSignalSpy loadProgressChangedSpy(webView(), SIGNAL(loadProgressChanged()));
    QVERIFY(waitForLoadSucceeded(webView()));

    QVERIFY(loadProgressChangedSpy.count() >= 1);

    QCOMPARE(webView()->loadProgress(), 100);
}
