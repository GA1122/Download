void FrameView::invalidateTreeIfNeededRecursive()
{
    lifecycle().advanceTo(DocumentLifecycle::InPaintInvalidation);
    invalidateTreeIfNeeded();
    lifecycle().advanceTo(DocumentLifecycle::PaintInvalidationClean);

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;

        toLocalFrame(child)->view()->invalidateTreeIfNeededRecursive();
    }
}
