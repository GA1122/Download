void webkit_web_frame_load_string(WebKitWebFrame* frame, const gchar* content, const gchar* contentMimeType, const gchar* contentEncoding, const gchar* baseUri)
{
    g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));
    g_return_if_fail(content);

    webkit_web_frame_load_data(frame, content, contentMimeType, contentEncoding, baseUri, NULL);
}
