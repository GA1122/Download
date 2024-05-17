GtkPolicyType webkit_web_frame_get_vertical_scrollbar_policy(WebKitWebFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), GTK_POLICY_AUTOMATIC);

    Frame* coreFrame = core(frame);
    FrameView* view = coreFrame->view();
    if (!view)
        return GTK_POLICY_AUTOMATIC;

    ScrollbarMode vMode = view->verticalScrollbarMode();

    if (vMode == ScrollbarAlwaysOn)
        return GTK_POLICY_ALWAYS;

    if (vMode == ScrollbarAlwaysOff)
        return GTK_POLICY_NEVER;

    return GTK_POLICY_AUTOMATIC;
}
