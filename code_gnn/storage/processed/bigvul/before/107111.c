void tst_QQuickWebView::removeFromCanvas()
{
    showWebView();

    QQuickItem* parent = webView()->parentItem();
    QQuickItem noCanvasItem;
    webView()->setParentItem(&noCanvasItem);
    QTest::qWait(200);
    webView()->setParentItem(parent);
    webView()->setVisible(true);
    QTest::qWait(200);
}
