void tst_QQuickWebView::showWebView()
{
    webView()->setSize(QSizeF(300, 400));

    webView()->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/direct-image-compositing.html")));
    QVERIFY(waitForLoadSucceeded(webView()));

    m_window->show();
    webView()->setVisible(true);
    QTest::qWait(200);
    webView()->setVisible(false);
    QTest::qWait(200);
}
