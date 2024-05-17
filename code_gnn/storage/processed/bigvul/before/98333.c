void FrameLoaderClient::frameLoaderDestroyed()
{
    webkit_web_frame_core_frame_gone(m_frame);
    g_object_unref(m_frame);
    m_frame = 0;
    delete this;
}
