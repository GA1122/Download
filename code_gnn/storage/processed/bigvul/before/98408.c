void webkit_web_frame_load_uri(WebKitWebFrame* frame, const gchar* uri)
{
    g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));
    g_return_if_fail(uri);

    Frame* coreFrame = core(frame);
    if (!coreFrame)
        return;

    coreFrame->loader()->load(ResourceRequest(KURL(KURL(), String::fromUTF8(uri))), false);
}
