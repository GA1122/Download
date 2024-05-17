void webkit_web_frame_load_alternate_string(WebKitWebFrame* frame, const gchar* content, const gchar* baseURL, const gchar* unreachableURL)
{
    g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));
    g_return_if_fail(content);

    webkit_web_frame_load_data(frame, content, NULL, NULL, baseURL, unreachableURL);
}
