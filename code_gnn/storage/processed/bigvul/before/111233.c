WebDOMNode WebPage::nodeAtDocumentPoint(const Platform::IntPoint& documentPoint)
{
    HitTestResult result = d->m_mainFrame->eventHandler()->hitTestResultAtPoint(WebCore::IntPoint(documentPoint), false);
    Node* node = result.innerNonSharedNode();
    return WebDOMNode(node);
}
