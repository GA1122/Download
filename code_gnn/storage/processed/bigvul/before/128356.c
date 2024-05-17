bool FrameView::shouldUseCustomScrollbars(Element*& customScrollbarElement, LocalFrame*& customScrollbarFrame)
{
    customScrollbarElement = 0;
    customScrollbarFrame = 0;

    if (Settings* settings = m_frame->settings()) {
        if (!settings->allowCustomScrollbarInMainFrame() && m_frame->isMainFrame())
            return false;
    }

    Document* doc = m_frame->document();

    Element* body = doc ? doc->body() : 0;
    if (body && body->renderer() && body->renderer()->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarElement = body;
        return true;
    }

    Element* docElement = doc ? doc->documentElement() : 0;
    if (docElement && docElement->renderer() && docElement->renderer()->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarElement = docElement;
        return true;
    }

    RenderPart* frameRenderer = m_frame->ownerRenderer();
    if (frameRenderer && frameRenderer->style()->hasPseudoStyle(SCROLLBAR)) {
        customScrollbarFrame = m_frame.get();
        return true;
    }

    return false;
}
