void FrameLoaderClient::dispatchDidReceiveTitle(const String& title)
{
    WebKitWebFramePrivate* priv = m_frame->priv;
    g_free(priv->title);
    priv->title = g_strdup(title.utf8().data());

    g_signal_emit_by_name(m_frame, "title-changed", priv->title);
    g_object_notify(G_OBJECT(m_frame), "title");

    WebKitWebView* webView = getViewFromFrame(m_frame);
    if (m_frame == webkit_web_view_get_main_frame(webView)) {
        g_signal_emit_by_name(webView, "title-changed", m_frame, title.utf8().data());
        g_object_notify(G_OBJECT(webView), "title");
    }
}
