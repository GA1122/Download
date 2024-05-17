bool Document::allowInlineEventHandlers(Node* node, EventListener* listener, const String& contextURL, const WTF::OrdinalNumber& contextLine)
{
    if (!contentSecurityPolicy()->allowInlineEventHandlers(contextURL, contextLine))
        return false;

    if (!m_frame)
        return false;
    if (!m_frame->script()->canExecuteScripts(NotAboutToExecuteScript))
        return false;
    if (node && node->document() != this && !node->document().allowInlineEventHandlers(node, listener, contextURL, contextLine))
        return false;

    return true;
}
