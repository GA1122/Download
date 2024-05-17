 void tst_QQuickWebView::scrollRequest()
 {
     webView()->setSize(QSizeF(300, 400));

    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForLoadSucceeded(webView()));

    int y = qRound(50 * webView()->page()->contentsScale());
    QVERIFY(webView()->experimental()->contentY() == y);
}
