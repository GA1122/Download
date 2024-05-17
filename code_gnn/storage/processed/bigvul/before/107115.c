void tst_QQuickWebView::stop()
{
    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForLoadSucceeded(webView()));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    webView()->stop();
}
