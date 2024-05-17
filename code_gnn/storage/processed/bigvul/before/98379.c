void webkit_web_frame_core_frame_gone(WebKitWebFrame* frame)
{
    ASSERT(WEBKIT_IS_WEB_FRAME(frame));
    frame->priv->coreFrame = 0;
}
