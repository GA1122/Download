void Document::setInPageCache(bool flag)
{
    if (m_inPageCache == flag)
        return;

    m_inPageCache = flag;

    FrameView* v = view();
    Page* page = this->page();

    if (flag) {
        ASSERT(!m_savedRenderer);
        m_savedRenderer = renderer();
        if (v) {
            v->cacheCurrentScrollPosition();
            if (page && page->mainFrame() == m_frame) {
                v->resetScrollbarsAndClearContentsSize();
                if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
                    scrollingCoordinator->clearStateTree();
            } else
                v->resetScrollbars();
        }
        m_styleRecalcTimer.stop();
    } else {
        ASSERT(!renderer() || renderer() == m_savedRenderer);
        ASSERT(m_renderArena);
        setRenderer(m_savedRenderer);
        m_savedRenderer = 0;

        if (childNeedsStyleRecalc())
            scheduleStyleRecalc();
    }
}
