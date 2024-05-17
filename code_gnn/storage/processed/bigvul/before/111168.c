IntRect WebPagePrivate::getRecursiveVisibleWindowRect(ScrollView* view, bool noClipOfMainFrame)
{
    ASSERT(m_mainFrame);

    if (noClipOfMainFrame && view == m_mainFrame->view()) {
        ASSERT_NOT_REACHED();
        return IntRect(IntPoint::zero(), view->contentsSize());
    }

    IntRect visibleWindowRect(view->contentsToWindow(view->visibleContentRect(false)));
    if (view->parent() && !(noClipOfMainFrame && view->parent() == m_mainFrame->view())) {
        visibleWindowRect.intersect(getRecursiveVisibleWindowRect(view->parent(), noClipOfMainFrame));
    }
    return visibleWindowRect;
}
