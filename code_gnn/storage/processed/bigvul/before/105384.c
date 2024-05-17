static void setVerticalScrollPolicy(WebKitWebView* webView, GtkScrollablePolicy policy)
{
    webView->priv->verticalScrollingPolicy = policy;
    gtk_widget_queue_resize(GTK_WIDGET(webView));
}
