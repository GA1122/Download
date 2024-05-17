void JPEGImageDecoder::jpegComplete()
{
    if (m_frameBufferCache.isEmpty())
        return;

    ImageFrame& buffer = m_frameBufferCache[0];
    buffer.setHasAlpha(false);
    buffer.setStatus(ImageFrame::FrameComplete);
}
