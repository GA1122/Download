void tst_QQuickWebView::reload()
{
    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForLoadSucceeded(webView()));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    webView()->reload();
    QVERIFY(waitForLoadSucceeded(webView()));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));
}
