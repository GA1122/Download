void FrameView::invalidateRect(const IntRect& rect)
{
    if (!parent()) {
        if (HostWindow* window = hostWindow())
            window->invalidateContentsAndRootView(rect);
        return;
    }

    RenderPart* renderer = m_frame->ownerRenderer();
    if (!renderer)
        return;

    IntRect paintInvalidationRect = rect;
    paintInvalidationRect.move(renderer->borderLeft() + renderer->paddingLeft(),
                     renderer->borderTop() + renderer->paddingTop());
    renderer->invalidatePaintRectangle(paintInvalidationRect);
}
