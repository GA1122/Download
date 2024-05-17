CanvasFontCache* Document::canvasFontCache()
{
    if (!m_canvasFontCache)
        m_canvasFontCache = CanvasFontCache::create(*this);

    return m_canvasFontCache.get();
}
