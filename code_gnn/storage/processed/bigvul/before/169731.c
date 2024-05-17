ScriptPromise ServiceWorkerContainer::registerServiceWorker(ScriptState* scriptState, const String& url, const RegistrationOptions& options)
{
    ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
    ScriptPromise promise = resolver->promise();

    if (!m_provider) {
        resolver->reject(DOMException::create(InvalidStateError, "Failed to register a ServiceWorker: The document is in an invalid state."));
        return promise;
    }

    ExecutionContext* executionContext = scriptState->getExecutionContext();
    if (!executionContext)
        return ScriptPromise();

    KURL scriptURL = enteredExecutionContext(scriptState->isolate())->completeURL(url);
    scriptURL.removeFragmentIdentifier();

    KURL patternURL;
    if (options.scope().isNull())
        patternURL = KURL(scriptURL, "./");
    else
        patternURL = enteredExecutionContext(scriptState->isolate())->completeURL(options.scope());

    registerServiceWorkerImpl(executionContext, scriptURL, patternURL, adoptPtr(new RegistrationCallback(resolver)));

    return promise;
}
