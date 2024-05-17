IntRect FrameView::convertToContainingView(const IntRect& localRect) const
{
    if (const ScrollView* parentScrollView = toScrollView(parent())) {
        if (parentScrollView->isFrameView()) {
            const FrameView* parentView = toFrameView(parentScrollView);
            RenderPart* renderer = m_frame->ownerRenderer();
            if (!renderer)
                return localRect;

            IntRect rect(localRect);
            rect.move(renderer->borderLeft() + renderer->paddingLeft(),
                renderer->borderTop() + renderer->paddingTop());
            return parentView->convertFromRenderer(*renderer, rect);
        }

        return Widget::convertToContainingView(localRect);
    }

    return localRect;
}
