int Document::requestIdleCallback(IdleRequestCallback* callback, const IdleRequestOptions& options)
{
    return ensureScriptedIdleTaskController().registerCallback(callback, options);
}
