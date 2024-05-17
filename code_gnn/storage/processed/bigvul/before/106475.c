void WebPageProxy::didCompleteRubberBandForMainFrame(const IntSize& initialOverhang)
{
    m_uiClient.didCompleteRubberBandForMainFrame(this, initialOverhang);
}
