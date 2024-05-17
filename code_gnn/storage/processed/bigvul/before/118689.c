void V8WindowShell::createContext()
{
    if (!m_frame->loader()->activeDocumentLoader())
        return;

    v8::Handle<v8::ObjectTemplate> globalTemplate = V8Window::GetShadowObjectTemplate(m_isolate, m_world->isMainWorld() ? MainWorld : IsolatedWorld);
    if (globalTemplate.IsEmpty())
        return;

    double contextCreationStartInSeconds = currentTime();

    if (DateExtension::get())
        ScriptController::registerExtensionIfNeeded(DateExtension::get());

    const V8Extensions& extensions = ScriptController::registeredExtensions();
    OwnPtr<const char*[]> extensionNames = adoptArrayPtr(new const char*[extensions.size()]);
    int index = 0;
    int extensionGroup = m_world->extensionGroup();
    int worldId = m_world->worldId();
    for (size_t i = 0; i < extensions.size(); ++i) {
        if (extensions[i] != DateExtension::get()
            && !m_frame->loader()->client()->allowScriptExtension(extensions[i]->name(), extensionGroup, worldId))
            continue;

        extensionNames[index++] = extensions[i]->name();
    }
    v8::ExtensionConfiguration extensionConfiguration(index, extensionNames.get());

    v8::HandleScope handleScope(m_isolate);
    m_context.set(m_isolate, v8::Context::New(m_isolate, &extensionConfiguration, globalTemplate, m_global.newLocal(m_isolate)));

    double contextCreationDurationInMilliseconds = (currentTime() - contextCreationStartInSeconds) * 1000;
    const char* histogramName = "WebCore.V8WindowShell.createContext.MainWorld";
    if (!m_world->isMainWorld())
        histogramName = "WebCore.V8WindowShell.createContext.IsolatedWorld";
    HistogramSupport::histogramCustomCounts(histogramName, contextCreationDurationInMilliseconds, 0, 10000, 50);
}
