ScriptPromise ServiceWorkerContainer::ready(ScriptState* callerState)
{
    if (!getExecutionContext())
        return ScriptPromise();

    if (!callerState->world().isMainWorld()) {
        return ScriptPromise::rejectWithDOMException(callerState, DOMException::create(NotSupportedError, "'ready' is only supported in pages."));
    }

    if (!m_ready) {
        m_ready = createReadyProperty();
        if (m_provider)
            m_provider->getRegistrationForReady(new GetRegistrationForReadyCallback(m_ready.get()));
    }

    return m_ready->promise(callerState->world());
}
