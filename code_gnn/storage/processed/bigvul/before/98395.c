gchar* webkit_web_frame_get_response_mime_type(WebKitWebFrame* frame)
{
    Frame* coreFrame = core(frame);
    WebCore::DocumentLoader* docLoader = coreFrame->loader()->documentLoader();
    String mimeType = docLoader->responseMIMEType();
    return g_strdup(mimeType.utf8().data());
}
