void FrameView::setTracksPaintInvalidations(bool trackPaintInvalidations)
{
    if (trackPaintInvalidations == m_isTrackingPaintInvalidations)
        return;

    for (Frame* frame = m_frame->tree().top(); frame; frame = frame->tree().traverseNext()) {
        if (!frame->isLocalFrame())
            continue;
        if (RenderView* renderView = toLocalFrame(frame)->contentRenderer())
            renderView->compositor()->setTracksPaintInvalidations(trackPaintInvalidations);
    }

    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("blink.invalidation"),
        "FrameView::setTracksPaintInvalidations", "enabled", trackPaintInvalidations);

    resetTrackedPaintInvalidations();
    m_isTrackingPaintInvalidations = trackPaintInvalidations;
}
