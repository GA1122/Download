gboolean webkit_web_view_can_show_mime_type(WebKitWebView* webView, const gchar* mimeType)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

    Frame* frame = core(webkit_web_view_get_main_frame(webView));
    if (FrameLoader* loader = frame->loader())
        return loader->canShowMIMEType(String::fromUTF8(mimeType));
    else
        return FALSE;
}
