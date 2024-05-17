WebKitSecurityOrigin* webkit_web_frame_get_security_origin(WebKitWebFrame* frame)
{
    WebKitWebFramePrivate* priv = frame->priv;
    if (!priv->coreFrame || !priv->coreFrame->document() || !priv->coreFrame->document()->securityOrigin())
        return NULL;

    if (priv->origin && priv->origin->priv->coreOrigin.get() == priv->coreFrame->document()->securityOrigin())
        return priv->origin;

    if (priv->origin)
        g_object_unref(priv->origin);

    priv->origin = kit(priv->coreFrame->document()->securityOrigin());
    return priv->origin;
}
