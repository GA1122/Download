void WebPagePrivate::setScrollOriginPoint(const Platform::IntPoint& documentScrollOrigin)
{
    m_inRegionScroller->d->reset();

    if (!m_hasInRegionScrollableAreas)
        return;

    postponeDocumentStyleRecalc();
    m_inRegionScroller->d->calculateInRegionScrollableAreasForPoint(documentScrollOrigin);
    if (!m_inRegionScroller->d->activeInRegionScrollableAreas().empty())
        m_client->notifyInRegionScrollableAreasChanged(m_inRegionScroller->d->activeInRegionScrollableAreas());
    resumeDocumentStyleRecalc();
}
