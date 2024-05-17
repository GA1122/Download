WebKitLoadStatus webkit_web_frame_get_load_status(WebKitWebFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), WEBKIT_LOAD_FINISHED);

    WebKitWebFramePrivate* priv = frame->priv;
    return priv->loadStatus;
}
