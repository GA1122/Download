void Document::didRemoveEventTargetNode(Node* handler)
{
    if (m_touchEventTargets && !m_touchEventTargets->isEmpty()) {
        if (handler == this)
            m_touchEventTargets->clear();
        else
            m_touchEventTargets->removeAll(handler);
        if (m_touchEventTargets->isEmpty() && parentDocument())
            parentDocument()->didRemoveEventTargetNode(this);
    }
}
