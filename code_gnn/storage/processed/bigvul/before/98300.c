void FrameLoaderClient::dispatchDidCommitLoad()
{
     
    g_object_freeze_notify(G_OBJECT(m_frame));

    WebKitWebFramePrivate* priv = m_frame->priv;
    g_free(priv->uri);
    priv->uri = g_strdup(core(m_frame)->loader()->url().prettyURL().utf8().data());
    g_free(priv->title);
    priv->title = NULL;
    g_object_notify(G_OBJECT(m_frame), "uri");
    g_object_notify(G_OBJECT(m_frame), "title");

    g_signal_emit_by_name(m_frame, "load-committed");
    notifyStatus(m_frame, WEBKIT_LOAD_COMMITTED);

    WebKitWebView* webView = getViewFromFrame(m_frame);
    if (m_frame == webkit_web_view_get_main_frame(webView)) {
        g_object_freeze_notify(G_OBJECT(webView));
        g_object_notify(G_OBJECT(webView), "uri");
        g_object_notify(G_OBJECT(webView), "title");
        g_object_thaw_notify(G_OBJECT(webView));
        g_signal_emit_by_name(webView, "load-committed", m_frame);
    }

    g_object_thaw_notify(G_OBJECT(m_frame));
}
