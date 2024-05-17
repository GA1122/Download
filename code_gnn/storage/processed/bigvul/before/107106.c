void tst_QQuickWebView::multipleWebViews()
{
    showWebView();

    QScopedPointer<QQuickWebView> webView1(newWebView());
    webView1->setParentItem(m_window->rootItem());
    QScopedPointer<QQuickWebView> webView2(newWebView());
    webView2->setParentItem(m_window->rootItem());

    webView1->setSize(QSizeF(300, 400));
    webView1->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForLoadSucceeded(webView1.data()));
    webView1->setVisible(true);

    webView2->setSize(QSizeF(300, 400));
    webView2->setUrl(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForLoadSucceeded(webView2.data()));
    webView2->setVisible(true);
     QTest::qWait(200);
 }
