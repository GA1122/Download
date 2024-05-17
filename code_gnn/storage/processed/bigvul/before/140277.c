void GIFImageReader::addFrameIfNecessary()
{
    if (m_frames.isEmpty() || m_frames.last()->isComplete())
        m_frames.append(adoptPtr(new GIFFrameContext(m_frames.size())));
}
