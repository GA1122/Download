void WorkerGlobalScopeFileSystem::webkitResolveLocalFileSystemURL(WorkerGlobalScope& worker, const String& url, PassOwnPtr<EntryCallback> successCallback, PassOwnPtr<ErrorCallback> errorCallback)
{
    KURL completedURL = worker.completeURL(url);
    ExecutionContext* secureContext = worker.executionContext();
    if (!secureContext->securityOrigin()->canAccessFileSystem() || !secureContext->securityOrigin()->canRequest(completedURL)) {
        DOMFileSystem::scheduleCallback(&worker, errorCallback, FileError::create(FileError::SECURITY_ERR));
        return;
    }

    if (!completedURL.isValid()) {
        DOMFileSystem::scheduleCallback(&worker, errorCallback, FileError::create(FileError::ENCODING_ERR));
        return;
    }

    LocalFileSystem::from(worker)->resolveURL(&worker, completedURL, ResolveURICallbacks::create(successCallback, errorCallback, &worker));
}
