void WebPageProxy::setViewNeedsDisplay(const IntRect& rect)
{
    m_pageClient->setViewNeedsDisplay(rect);
}
