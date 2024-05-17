AtkObject* webkit_web_frame_get_focused_accessible_element(WebKitWebFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), NULL);

#if HAVE(ACCESSIBILITY)
    if (!AXObjectCache::accessibilityEnabled())
        AXObjectCache::enableAccessibility();

    WebKitWebFramePrivate* priv = frame->priv;
    if (!priv->coreFrame || !priv->coreFrame->document())
        return NULL;

    RenderView* root = toRenderView(priv->coreFrame->document()->renderer());
    if (!root)
        return NULL;

    AtkObject* wrapper =  priv->coreFrame->document()->axObjectCache()->getOrCreate(root)->wrapper();
    if (!wrapper)
        return NULL;

    return webkit_accessible_get_focused_element(WEBKIT_ACCESSIBLE(wrapper));
#else
    return NULL;
#endif
}
