void XMLHttpRequest::internalAbort(DropProtection async)
{
    m_error = true;

    m_receivedLength = 0;
    m_decoder = 0;

    InspectorInstrumentation::didFailXHRLoading(scriptExecutionContext(), this);

    if (m_responseStream && m_state != DONE)
        m_responseStream->abort();

    if (!m_loader)
        return;

    m_loader->cancel();
    m_loader = 0;

    if (async == DropProtectionAsync)
        dropProtectionSoon();
    else
        dropProtection();
}
