ServiceWorkerContainer* NavigatorServiceWorker::serviceWorker(ExceptionState& exceptionState)
{
    if (frame() && !frame()->securityContext()->securityOrigin()->canAccessServiceWorkers()) {
        if (frame()->securityContext()->isSandboxed(SandboxOrigin))
            exceptionState.throwSecurityError("Service worker is disabled because the context is sandboxed and lacks the 'allow-same-origin' flag.");
        else
            exceptionState.throwSecurityError("Access to service workers is denied in this document origin.");
        return nullptr;
    }
    if (!m_serviceWorker && frame()) {
        ASSERT(frame()->domWindow());
        m_serviceWorker = ServiceWorkerContainer::create(frame()->domWindow()->executionContext());
    }
    return m_serviceWorker.get();
}
