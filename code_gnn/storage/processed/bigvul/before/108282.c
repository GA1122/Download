void FrameLoader::stop()
{
    RefPtr<Frame> protector(m_frame);
    
    if (m_frame->document()->parser())
        m_frame->document()->parser()->stopParsing();
    m_frame->document()->finishParsing();

    if (m_iconLoader)
        m_iconLoader->stopLoading();
}
