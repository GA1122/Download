void Document::layoutUpdated()
{
    if (frame() && frame()->page())
        frame()->page()->chromeClient().layoutUpdated(frame());

    markers().updateRenderedRectsForMarkers();

    if (isRenderingReady() && body() && !styleEngine().hasPendingSheets()) {
        if (!m_documentTiming.firstLayout())
            m_documentTiming.markFirstLayout();
    }
}
