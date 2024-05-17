void FrameLoaderClient::dispatchDidStartProvisionalLoad()
{
    notifyStatus(m_frame, WEBKIT_LOAD_PROVISIONAL);
}
