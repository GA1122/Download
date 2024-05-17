void WebPageProxy::setFindIndicator(const FloatRect& selectionRectInWindowCoordinates, const Vector<FloatRect>& textRectsInSelectionRectCoordinates, const ShareableBitmap::Handle& contentImageHandle, bool fadeOut)
{
    RefPtr<FindIndicator> findIndicator = FindIndicator::create(selectionRectInWindowCoordinates, textRectsInSelectionRectCoordinates, contentImageHandle);
    m_pageClient->setFindIndicator(findIndicator.release(), fadeOut);
}
