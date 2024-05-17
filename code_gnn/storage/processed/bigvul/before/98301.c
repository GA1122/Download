void FrameLoaderClient::dispatchDidFailLoad(const ResourceError& error)
{
    notifyStatus(m_frame, WEBKIT_LOAD_FAILED);

    WebKitWebView* webView = getViewFromFrame(m_frame);
    GError* webError = g_error_new_literal(g_quark_from_string(error.domain().utf8().data()),
                                           error.errorCode(),
                                           error.localizedDescription().utf8().data());
    gboolean isHandled = false;
    g_signal_emit_by_name(webView, "load-error", m_frame, error.failingURL().utf8().data(), webError, &isHandled);

    if (isHandled) {
        g_error_free(webError);
        return;
    }

    if (!shouldFallBack(error)) {
        g_error_free(webError);
        return;
    }

    String content;
    gchar* fileContent = 0;
    gchar* errorURI = g_filename_to_uri(DATA_DIR"/webkit-1.0/resources/error.html", NULL, NULL);
    GFile* errorFile = g_file_new_for_uri(errorURI);
    g_free(errorURI);

    if (!errorFile)
        content = String::format("<html><body>%s</body></html>", webError->message);
    else {
        gboolean loaded = g_file_load_contents(errorFile, 0, &fileContent, 0, 0, 0);
        if (!loaded)
            content = String::format("<html><body>%s</body></html>", webError->message);
        else
            content = String::format(fileContent, error.failingURL().utf8().data(), webError->message);
    }

    webkit_web_frame_load_alternate_string(m_frame, content.utf8().data(), 0, error.failingURL().utf8().data());

    g_free(fileContent);

    if (errorFile)
        g_object_unref(errorFile);

    g_error_free(webError);
}
