static void paintWebView(Frame* frame, gboolean transparent, GraphicsContext& context, const IntRect& clipRect, const Vector<IntRect>& rects)
{
    bool coalesce = true;

    if (rects.size() > 0)
        coalesce = shouldCoalesce(clipRect, rects);

    if (coalesce) {
        context.clip(clipRect);
        if (transparent)
            context.clearRect(clipRect);
        frame->view()->paint(&context, clipRect);
    } else {
        for (size_t i = 0; i < rects.size(); i++) {
            IntRect rect = rects[i];
            context.save();
            context.clip(rect);
            if (transparent)
                context.clearRect(rect);
            frame->view()->paint(&context, rect);
            context.restore();
        }
    }

    context.save();
    context.clip(clipRect);
    frame->page()->inspectorController()->drawNodeHighlight(context);
    context.restore();
}
