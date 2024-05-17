void Document::setAutofocusElement(Element* element)
{
    if (!element) {
        m_autofocusElement = nullptr;
        return;
    }
    if (m_hasAutofocused)
        return;
    m_hasAutofocused = true;
    DCHECK(!m_autofocusElement);
    m_autofocusElement = element;
    m_taskRunner->postTask(BLINK_FROM_HERE, AutofocusTask::create());
}
