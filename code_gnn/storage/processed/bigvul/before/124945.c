void RenderBox::scrollRectToVisible(const LayoutRect& rect, const ScrollAlignment& alignX, const ScrollAlignment& alignY)
{
    DisableCompositingQueryAsserts disabler;

    RenderBox* parentBox = 0;
    LayoutRect newRect = rect;

    bool restrictedByLineClamp = false;
    if (parent()) {
        parentBox = parent()->enclosingBox();
        restrictedByLineClamp = !parent()->style()->lineClamp().isNone();
    }

    if (hasOverflowClip() && !restrictedByLineClamp) {
        newRect = layer()->scrollableArea()->exposeRect(rect, alignX, alignY);
    } else if (!parentBox && canBeProgramaticallyScrolled()) {
        if (FrameView* frameView = this->frameView()) {
            Element* ownerElement = document().ownerElement();

            if (ownerElement && ownerElement->renderer()) {
                HTMLFrameElementBase* frameElementBase = 0;

                if (isHTMLFrameElement(*ownerElement) || isHTMLIFrameElement(*ownerElement))
                    frameElementBase = toHTMLFrameElementBase(ownerElement);

                if (frameElementAndViewPermitScroll(frameElementBase, frameView)) {
                    LayoutRect viewRect = frameView->visibleContentRect();
                    LayoutRect exposeRect = ScrollAlignment::getRectToExpose(viewRect, rect, alignX, alignY);

                    int xOffset = roundToInt(exposeRect.x());
                    int yOffset = roundToInt(exposeRect.y());
                    xOffset = max(0, min(frameView->contentsWidth(), xOffset));
                    yOffset = max(0, min(frameView->contentsHeight(), yOffset));

                    frameView->setScrollPosition(IntPoint(xOffset, yOffset));
                    if (frameView->safeToPropagateScrollToParent()) {
                        parentBox = ownerElement->renderer()->enclosingBox();
                        newRect.setX(rect.x() - frameView->scrollX() + frameView->x());
                        newRect.setY(rect.y() - frameView->scrollY() + frameView->y());
                    } else {
                        parentBox = 0;
                    }
                }
            } else {
                LayoutRect viewRect = frameView->visibleContentRect();
                LayoutRect r = ScrollAlignment::getRectToExpose(viewRect, rect, alignX, alignY);
                frameView->setScrollPosition(roundedIntPoint(r.location()));
            }
        }
    }

    if (frame()->page()->autoscrollController().autoscrollInProgress())
        parentBox = enclosingScrollableBox();

    if (parentBox)
        parentBox->scrollRectToVisible(newRect, alignX, alignY);
}
