void FrameLoaderClient::setTitle(const String& title, const KURL& url)
{
    WebKitWebFramePrivate* frameData = WEBKIT_WEB_FRAME_GET_PRIVATE(m_frame);
    g_free(frameData->title);
    frameData->title = g_strdup(title.utf8().data());
}
