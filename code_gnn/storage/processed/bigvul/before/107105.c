void tst_QQuickWebView::multipleWebViewWindows()
{
    showWebView();

    QQuickWebView* webView1 = newWebView();
    QScopedPointer<TestWindow> window1(new TestWindow(webView1));
    QQuickWebView* webView2 = newWebView();
    QScopedPointer<TestWindow> window2(new TestWindow(webView2));

    webView1->setSize(QSizeF(300, 400));
    webView1->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForLoadSucceeded(webView1));
    window1->show();
    webView1->setVisible(true);

    webView2->setSize(QSizeF(300, 400));
    webView2->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForLoadSucceeded(webView2));
    window2->show();
    webView2->setVisible(true);
    QTest::qWait(200);
}
