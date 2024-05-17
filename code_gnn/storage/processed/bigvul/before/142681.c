void FrameLoader::dispatchDidClearDocumentOfWindowObject()
{
    if (!m_frame->script().canExecuteScripts(NotAboutToExecuteScript))
        return;

    InspectorInstrumentation::didClearDocumentOfWindowObject(m_frame);

    if (m_dispatchingDidClearWindowObjectInMainWorld)
        return;
    TemporaryChange<bool>
        inDidClearWindowObject(m_dispatchingDidClearWindowObjectInMainWorld, true);
    client()->dispatchDidClearWindowObjectInMainWorld();
}
