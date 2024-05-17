static void webkit_web_frame_init(WebKitWebFrame* frame)
{
    WebKitWebFramePrivate* priv = WEBKIT_WEB_FRAME_GET_PRIVATE(frame);

    frame->priv = priv;
}
