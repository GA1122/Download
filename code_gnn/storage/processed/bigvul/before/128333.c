void FrameView::scrollbarStyleChanged()
{
    if (!m_frame->isMainFrame())
        return;
    ScrollView::scrollbarStyleChanged();
}
