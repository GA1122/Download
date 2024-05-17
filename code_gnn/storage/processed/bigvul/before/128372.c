bool FrameView::usesCompositedScrolling() const
{
    RenderView* renderView = this->renderView();
    if (!renderView)
        return false;
    if (m_frame->settings() && m_frame->settings()->preferCompositingToLCDTextEnabled())
        return renderView->compositor()->inCompositingMode();
    return false;
}
