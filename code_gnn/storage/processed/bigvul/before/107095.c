void tst_QQuickWebView::accessPage()
{
    QQuickWebPage* const pageDirectAccess = webView()->page();

    QVariant pagePropertyValue = webView()->experimental()->property("page");
    QQuickWebPage* const pagePropertyAccess = pagePropertyValue.value<QQuickWebPage*>();
    QCOMPARE(pagePropertyAccess, pageDirectAccess);
}
