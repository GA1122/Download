bool GIFImageReader::decode(size_t frameIndex)
{
    m_globalColorMap.buildTable(data(0), m_data->size());

    bool frameDecoded = false;
    GIFFrameContext* currentFrame = m_frames[frameIndex].get();

    return currentFrame->decode(data(0), m_data->size(), m_client, &frameDecoded)
        && (!frameDecoded || m_client->frameComplete(frameIndex));
}
