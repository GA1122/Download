void InputHandler::focusedNodeChanged()
{
    ASSERT(m_webPage->m_page->focusController());
    Frame* frame = m_webPage->m_page->focusController()->focusedOrMainFrame();
    if (!frame || !frame->document())
        return;

    Node* node = frame->document()->focusedNode();

    if (isActiveTextEdit() && m_currentFocusElement == node) {
        notifyClientOfKeyboardVisibilityChange(true);
        return;
    }

    if (node && node->isElementNode()) {
        Element* element = static_cast<Element*>(node);
        if (DOMSupport::isElementTypePlugin(element)) {
            setPluginFocused(element);
            return;
        }

        if (DOMSupport::isTextBasedContentEditableElement(element)) {
            setElementFocused(element);
            return;
        }
    }

    if (isActiveTextEdit() && m_currentFocusElement->isContentEditable()) {

        if (processingChange())
            return;

        Frame* frame = m_currentFocusElement->document()->frame();
        ASSERT(frame);

        if (frame == m_webPage->focusedOrMainFrame() && frame->selection()->start().anchorNode()
            && frame->selection()->start().anchorNode()->isContentEditable())
                return;
    }

    setElementUnfocused();
}
