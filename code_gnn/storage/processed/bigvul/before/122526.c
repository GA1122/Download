bool InspectorClientImpl::overridesShowPaintRects()
{
    return m_inspectedWebView->isAcceleratedCompositingActive();
}
