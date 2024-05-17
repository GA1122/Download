bool WebPage::setNodeFocus(const WebDOMNode& node, bool on)
{
    Node* nodeImpl = node.impl();

    if (nodeImpl && nodeImpl->isFocusable()) {
        Document* doc = nodeImpl->document();
        if (Page* page = doc->page()) {
            if (on) {
                page->focusController()->setFocusedNode(nodeImpl, doc->frame());
                if (nodeImpl->isElementNode())
                    static_cast<Element*>(nodeImpl)->updateFocusAppearance(true);
                d->m_inputHandler->didNodeOpenPopup(nodeImpl);
            } else if (doc->focusedNode() == nodeImpl)  
                page->focusController()->setFocusedNode(0, doc->frame());

            return true;
        }
    }
    return false;
}
