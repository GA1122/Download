void FrameLoader::dispatchUnloadEvent()
{
    NavigationCounterForUnload counter;

    m_protectProvisionalLoader = false;
    saveScrollState();

    if (m_frame->document() && !SVGImage::isInSVGImage(m_frame->document()))
        m_frame->document()->dispatchUnloadEvents();

    if (Page* page = m_frame->page())
        page->undoStack().didUnloadFrame(*m_frame);
}
