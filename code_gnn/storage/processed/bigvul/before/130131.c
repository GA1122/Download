int HTMLBodyElement::scrollLeft()
{
    Document& document = this->document();
    document.updateLayoutIgnorePendingStylesheets();

    if (RuntimeEnabledFeatures::scrollTopLeftInteropEnabled()) {
        RenderBox* render = renderBox();
        if (!render)
            return 0;
        if (render->hasOverflowClip())
            return adjustForAbsoluteZoom(render->scrollLeft(), render);
        if (!document.inQuirksMode())
            return 0;
    }

    FrameView* view = document.view();
    return view ? adjustForZoom(view->scrollX(), &document) : 0;
}
