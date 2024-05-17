void FrameView::calculateScrollbarModesForLayoutAndSetViewportRenderer(ScrollbarMode& hMode, ScrollbarMode& vMode, ScrollbarModesCalculationStrategy strategy)
{
    m_viewportRenderer = 0;

    const HTMLFrameOwnerElement* owner = m_frame->deprecatedLocalOwner();
    if (owner && (owner->scrollingMode() == ScrollbarAlwaysOff)) {
        hMode = ScrollbarAlwaysOff;
        vMode = ScrollbarAlwaysOff;
        return;
    }

    if (m_canHaveScrollbars || strategy == RulesFromWebContentOnly) {
        hMode = ScrollbarAuto;
        vMode = ScrollbarAuto;
    } else {
        hMode = ScrollbarAlwaysOff;
        vMode = ScrollbarAlwaysOff;
    }

    if (!isSubtreeLayout()) {
        Document* document = m_frame->document();
        Node* body = document->body();
        if (isHTMLFrameSetElement(body) && body->renderer()) {
            vMode = ScrollbarAlwaysOff;
            hMode = ScrollbarAlwaysOff;
        } else if (Element* viewportElement = document->viewportDefiningElement()) {
            if (RenderObject* viewportRenderer = viewportElement->renderer()) {
                if (viewportRenderer->style())
                    applyOverflowToViewportAndSetRenderer(viewportRenderer, hMode, vMode);
            }
        }
    }
}
