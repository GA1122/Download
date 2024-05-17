gchar* webkit_web_frame_dump_render_tree(WebKitWebFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), NULL);

    Frame* coreFrame = core(frame);
    if (!coreFrame)
        return g_strdup("");

    FrameView* view = coreFrame->view();

    if (view && view->layoutPending())
        view->layout();

    String string = externalRepresentation(coreFrame->contentRenderer());
    return g_strdup(string.utf8().data());
}
