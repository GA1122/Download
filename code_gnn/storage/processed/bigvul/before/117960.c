void ScriptController::namedItemAdded(HTMLDocument* doc, const AtomicString& name)
{
    m_proxy->windowShell()->namedItemAdded(doc, name);
}
