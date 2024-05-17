bool CCLayerTreeHost::compositeAndReadback(void *pixels, const IntRect& rect)
{
    return m_proxy->compositeAndReadback(pixels, rect);
}
