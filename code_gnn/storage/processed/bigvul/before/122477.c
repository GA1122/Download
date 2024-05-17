void InspectorController::inspect(Node* node)
{
    if (!node)
        return;
    Document* document = node->ownerDocument();
    if (!document)
        return;
    LocalFrame* frame = document->frame();
    if (!frame)
        return;

    if (node->nodeType() != Node::ELEMENT_NODE && node->nodeType() != Node::DOCUMENT_NODE)
        node = node->parentNode();

    InjectedScript injectedScript = m_injectedScriptManager->injectedScriptFor(ScriptState::forMainWorld(frame));
    if (injectedScript.isEmpty())
        return;
    injectedScript.inspectNode(node);
}
