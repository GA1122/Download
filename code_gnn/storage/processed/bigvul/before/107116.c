void tst_QQuickWebView::stopEnabledAfterLoadStarted()
{
    QCOMPARE(webView()->loading(), false);

    LoadStartedCatcher catcher(webView());
    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    waitForSignal(&catcher, SIGNAL(finished()));

    QCOMPARE(webView()->loading(), true);

    QVERIFY(waitForLoadSucceeded(webView()));
}
