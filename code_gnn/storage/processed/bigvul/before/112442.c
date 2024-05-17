void Document::didRemoveEventTargetNode(Node* handler)
{
    if (m_touchEventTargets) {
        m_touchEventTargets->removeAll(handler);
        if ((handler == this || m_touchEventTargets->isEmpty()) && parentDocument())
            parentDocument()->didRemoveEventTargetNode(this);
    }
}
