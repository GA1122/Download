void TouchEventHandler::drawTapHighlight()
{
    Element* elementUnderFatFinger = m_lastFatFingersResult.nodeAsElementIfApplicable();
    if (!elementUnderFatFinger)
        return;

    Element* element = elementForTapHighlight(elementUnderFatFinger);
    if (!element)
        return;

    RenderObject* renderer = element->renderer();
    ASSERT(renderer);

    Frame* elementFrame = element->document()->frame();
    ASSERT(elementFrame);

    FrameView* elementFrameView = elementFrame->view();
    if (!elementFrameView)
        return;

    RenderLayer* layer = m_webPage->enclosingFixedPositionedAncestorOrSelfIfFixedPositioned(renderer->enclosingLayer());
    bool shouldHideTapHighlightRightAfterScrolling = !layer->renderer()->isRenderView();
    shouldHideTapHighlightRightAfterScrolling |= !!m_webPage->m_inRegionScroller->d->isActive();

    IntPoint framePos(m_webPage->frameOffset(elementFrame));

    IntRect absoluteRect(renderer->absoluteClippedOverflowRect());
    absoluteRect.move(framePos.x(), framePos.y());

    IntRect clippingRect;
    if (elementFrame == m_webPage->mainFrame())
        clippingRect = IntRect(IntPoint(0, 0), elementFrameView->contentsSize());
    else
        clippingRect = m_webPage->mainFrame()->view()->windowToContents(m_webPage->getRecursiveVisibleWindowRect(elementFrameView, true  ));
    clippingRect = intersection(absoluteRect, clippingRect);

    Vector<FloatQuad> focusRingQuads;
    renderer->absoluteFocusRingQuads(focusRingQuads);

    Platform::IntRectRegion region;
    for (size_t i = 0; i < focusRingQuads.size(); ++i) {
        IntRect rect = focusRingQuads[i].enclosingBoundingBox();
        rect.move(framePos.x(), framePos.y());
        IntRect clippedRect = intersection(clippingRect, rect);
        if (clippedRect.isEmpty())
            continue;
        clippedRect.inflate(2);
        region = unionRegions(region, Platform::IntRect(clippedRect));
    }

    Color highlightColor = element->renderStyle()->tapHighlightColor();

    m_webPage->m_tapHighlight->draw(region,
                                    highlightColor.red(), highlightColor.green(), highlightColor.blue(), highlightColor.alpha(),
                                    shouldHideTapHighlightRightAfterScrolling);
}
