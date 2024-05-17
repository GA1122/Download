void ScriptController::namedItemRemoved(HTMLDocument* doc, const AtomicString& name)
{
    m_proxy->windowShell()->namedItemRemoved(doc, name);
}
