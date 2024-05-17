void WebPagePrivate::setNeedsLayout()
{
    FrameView* view = m_mainFrame->view();
    ASSERT(view);
    view->setNeedsLayout();
}
