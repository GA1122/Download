IntPoint FrameView::convertToContainingView(const IntPoint& localPoint) const
{
    if (const ScrollView* parentScrollView = toScrollView(parent())) {
        if (parentScrollView->isFrameView()) {
            const FrameView* parentView = toFrameView(parentScrollView);

            RenderPart* renderer = m_frame->ownerRenderer();
            if (!renderer)
                return localPoint;

            IntPoint point(localPoint);

            point.move(renderer->borderLeft() + renderer->paddingLeft(),
                       renderer->borderTop() + renderer->paddingTop());
            return parentView->convertFromRenderer(*renderer, point);
        }

        return Widget::convertToContainingView(localPoint);
    }

    return localPoint;
}
