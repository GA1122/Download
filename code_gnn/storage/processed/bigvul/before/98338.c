static void loadDone(WebKitWebFrame* frame, bool didSucceed)
{
    g_signal_emit_by_name(frame, "load-done", didSucceed);
    notifyStatus(frame, WEBKIT_LOAD_FINISHED);
}
