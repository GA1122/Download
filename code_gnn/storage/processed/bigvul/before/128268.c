bool FrameView::isInPerformLayout() const
{
    ASSERT(m_inPerformLayout == (lifecycle().state() == DocumentLifecycle::InPerformLayout));
    return m_inPerformLayout;
}
