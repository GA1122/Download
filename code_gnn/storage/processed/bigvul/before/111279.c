bool WebPage::pinchZoomAboutPoint(double scale, const Platform::FloatPoint& documentFocalPoint)
{
    d->m_userPerformedManualZoom = true;
    d->m_userPerformedManualScroll = true;

    if (d->m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled) {
        d->setTextReflowAnchorPoint(webkitThreadViewportAccessor()->roundedDocumentContents(documentFocalPoint));

        if (!d->m_currentPinchZoomNode)
            return false;
    }

    return d->zoomAboutPoint(scale, documentFocalPoint);
}
