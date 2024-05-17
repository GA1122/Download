void reportFailure(ExecutionContext*, PassOwnPtr<AsyncFileSystemCallbacks> callbacks, FileError::ErrorCode error)
{
    callbacks->didFail(error);
}
