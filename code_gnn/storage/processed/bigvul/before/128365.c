void FrameView::updateLayoutAndStyleIfNeededRecursive()
{

    m_frame->document()->updateRenderTreeIfNeeded();

    if (needsLayout())
        layout();

    Vector<RefPtr<FrameView> > frameViews;
    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(child)->view())
            frameViews.append(view);
    }

    const Vector<RefPtr<FrameView> >::iterator end = frameViews.end();
    for (Vector<RefPtr<FrameView> >::iterator it = frameViews.begin(); it != end; ++it)
        (*it)->updateLayoutAndStyleIfNeededRecursive();

    if (m_frame->document()->hasSVGFilterElementsRequiringLayerUpdate()) {
        m_frame->document()->updateRenderTreeIfNeeded();

        if (needsLayout())
            layout();
    }

    ASSERT(!needsLayout());
    ASSERT(!m_frame->document()->hasSVGFilterElementsRequiringLayerUpdate());
#if ENABLE(ASSERT)
    m_frame->document()->renderView()->assertRendererLaidOut();
#endif

}
