bool WebPage::getNodeRect(const WebDOMNode& node, Platform::IntRect& result)
{
    Node* nodeImpl = node.impl();
    if (nodeImpl && nodeImpl->renderer()) {
        result = nodeImpl->getRect();
        return true;
    }

    return false;
}
