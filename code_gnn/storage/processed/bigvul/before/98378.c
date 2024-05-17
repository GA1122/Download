void webkit_web_frame_clear_main_frame_name(WebKitWebFrame* frame)
{
    g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

    core(frame)->tree()->clearName();
}
