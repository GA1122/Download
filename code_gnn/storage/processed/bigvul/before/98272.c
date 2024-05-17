void FrameLoaderClient::assignIdentifierToInitialRequest(unsigned long identifier, WebCore::DocumentLoader*, const ResourceRequest& request)
{
    webkit_web_view_add_resource(getViewFromFrame(m_frame), toString(identifier),
                                 WEBKIT_WEB_RESOURCE(g_object_new(WEBKIT_TYPE_WEB_RESOURCE, "uri", request.url().string().utf8().data(), 0)));
}
