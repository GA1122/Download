void FrameLoaderClient::forceLayout()
{
    FrameView* view = core(m_frame)->view();
    if (view)
        view->forceLayout(true);
}
