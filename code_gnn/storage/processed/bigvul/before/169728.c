ScriptPromise ServiceWorkerContainer::getRegistrations(ScriptState* scriptState)
{
    ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
    ScriptPromise promise = resolver->promise();

    if (!m_provider) {
        resolver->reject(DOMException::create(InvalidStateError, "Failed to get ServiceWorkerRegistration objects: The document is in an invalid state."));
        return promise;
    }

    ExecutionContext* executionContext = scriptState->getExecutionContext();
    RefPtr<SecurityOrigin> documentOrigin = executionContext->getSecurityOrigin();
    String errorMessage;
    if (!executionContext->isSecureContext(errorMessage)) {
        resolver->reject(DOMException::create(SecurityError, errorMessage));
        return promise;
    }

    KURL pageURL = KURL(KURL(), documentOrigin->toString());
    if (!SchemeRegistry::shouldTreatURLSchemeAsAllowingServiceWorkers(pageURL.protocol())) {
        resolver->reject(DOMException::create(SecurityError, "Failed to get ServiceWorkerRegistration objects: The URL protocol of the current origin ('" + documentOrigin->toString() + "') is not supported."));
        return promise;
    }

    m_provider->getRegistrations(new GetRegistrationsCallback(resolver));

    return promise;
}
