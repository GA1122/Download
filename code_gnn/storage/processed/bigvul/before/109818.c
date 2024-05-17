void Document::setNeedsFocusedElementCheck()
{
    if (!m_focusedElement || m_didPostCheckFocusedElementTask)
        return;
    postTask(CheckFocusedElementTask::create());
    m_didPostCheckFocusedElementTask = true;
}
