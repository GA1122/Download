static void setHorizontalScrollPolicy(WebKitWebView* webView, GtkScrollablePolicy policy)
{
    webView->priv->horizontalScrollingPolicy = policy;
    gtk_widget_queue_resize(GTK_WIDGET(webView));
}
