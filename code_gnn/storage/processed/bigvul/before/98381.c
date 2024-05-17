static void webkit_web_frame_finalize(GObject* object)
{
    WebKitWebFrame* frame = WEBKIT_WEB_FRAME(object);
    WebKitWebFramePrivate* priv = frame->priv;

    if (priv->coreFrame) {
        priv->coreFrame->loader()->cancelAndClear();
        priv->coreFrame = 0;
    }

    g_free(priv->name);
    g_free(priv->title);
    g_free(priv->uri);

    G_OBJECT_CLASS(webkit_web_frame_parent_class)->finalize(object);
}
