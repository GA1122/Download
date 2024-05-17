void FrameView::updateScrollCorner()
{
    RefPtr<RenderStyle> cornerStyle;
    IntRect cornerRect = scrollCornerRect();
    Document* doc = m_frame->document();

    if (doc && !cornerRect.isEmpty()) {
        if (Element* body = doc->body()) {
            if (RenderObject* renderer = body->renderer())
                cornerStyle = renderer->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), renderer->style());
        }

        if (!cornerStyle) {
            if (Element* docElement = doc->documentElement()) {
                if (RenderObject* renderer = docElement->renderer())
                    cornerStyle = renderer->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), renderer->style());
            }
        }

        if (!cornerStyle) {
            if (RenderPart* renderer = m_frame->ownerRenderer())
                cornerStyle = renderer->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), renderer->style());
        }
    }

    if (cornerStyle) {
        if (!m_scrollCorner)
            m_scrollCorner = RenderScrollbarPart::createAnonymous(doc);
        m_scrollCorner->setStyle(cornerStyle.release());
        invalidateScrollCorner(cornerRect);
    } else if (m_scrollCorner) {
        m_scrollCorner->destroy();
        m_scrollCorner = nullptr;
    }

    ScrollView::updateScrollCorner();
}
