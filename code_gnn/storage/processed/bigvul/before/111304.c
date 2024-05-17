void WebPagePrivate::requestLayoutIfNeeded() const
{
    FrameView* view = m_mainFrame->view();
    ASSERT(view);
    view->updateLayoutAndStyleIfNeededRecursive();
    ASSERT(!view->needsLayout());
}
