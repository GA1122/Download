bool WebPage::setNodeHovered(const WebDOMNode& node, bool on)
{
    if (Node* nodeImpl = node.impl()) {
        nodeImpl->setHovered(on);
        return true;
    }
    return false;
}
