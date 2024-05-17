void FrameLoaderClient::dispatchDidFirstVisuallyNonEmptyLayout()
{
    notifyStatus(m_frame, WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT);
}
