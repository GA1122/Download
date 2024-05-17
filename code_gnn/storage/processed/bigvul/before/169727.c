ScriptPromise ServiceWorkerContainer::getRegistration(ScriptState* scriptState, const String& documentURL)
{
    ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
    ScriptPromise promise = resolver->promise();

    if (!m_provider) {
        resolver->reject(DOMException::create(InvalidStateError, "Failed to get a ServiceWorkerRegistration: The document is in an invalid state."));
        return promise;
    }

    ExecutionContext* executionContext = scriptState->getExecutionContext();
    if (!executionContext)
        return ScriptPromise();

    RefPtr<SecurityOrigin> documentOrigin = executionContext->getSecurityOrigin();
    String errorMessage;
    if (!executionContext->isSecureContext(errorMessage)) {
        resolver->reject(DOMException::create(SecurityError, errorMessage));
        return promise;
    }

    KURL pageURL = KURL(KURL(), documentOrigin->toString());
    if (!SchemeRegistry::shouldTreatURLSchemeAsAllowingServiceWorkers(pageURL.protocol())) {
        resolver->reject(DOMException::create(SecurityError, "Failed to get a ServiceWorkerRegistration: The URL protocol of the current origin ('" + documentOrigin->toString() + "') is not supported."));
        return promise;
    }

    KURL completedURL = enteredExecutionContext(scriptState->isolate())->completeURL(documentURL);
    completedURL.removeFragmentIdentifier();
    if (!documentOrigin->canRequest(completedURL)) {
        RefPtr<SecurityOrigin> documentURLOrigin = SecurityOrigin::create(completedURL);
        resolver->reject(DOMException::create(SecurityError, "Failed to get a ServiceWorkerRegistration: The origin of the provided documentURL ('" + documentURLOrigin->toString() + "') does not match the current origin ('" + documentOrigin->toString() + "')."));
        return promise;
    }
    m_provider->getRegistration(completedURL, new GetRegistrationCallback(resolver));

    return promise;
}
