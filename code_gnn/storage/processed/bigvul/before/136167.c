void Document::layoutUpdated()
{
    markers().updateRenderedRectsForMarkers();

    if (isRenderingReady() && body() && !styleEngine().hasPendingSheets()) {
        if (!m_documentTiming.firstLayout())
            m_documentTiming.markFirstLayout();
    }
}
