void LocalFileSystem::requestFileSystemAccessInternal(ExecutionContext* context, const Closure& allowed, const Closure& denied)
{
    if (!client()) {
        denied();
        return;
    }
    if (!context->isDocument()) {
        if (!client()->requestFileSystemAccessSync(context)) {
            denied();
            return;
        }
        allowed();
        return;
    }
    client()->requestFileSystemAccessAsync(context, PermissionCallbacks::create(allowed, denied));
}
