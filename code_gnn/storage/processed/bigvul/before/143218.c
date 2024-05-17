void Document::layoutUpdated()
{
    if (frame() && frame()->page())
        frame()->page()->chromeClient().layoutUpdated(frame());

    markers().invalidateRectsForAllMarkers();

    if (isRenderingReady() && body() && !styleEngine().hasPendingScriptBlockingSheets()) {
        if (!m_documentTiming.firstLayout())
            m_documentTiming.markFirstLayout();
    }

    m_rootScrollerController->didUpdateLayout();
}
