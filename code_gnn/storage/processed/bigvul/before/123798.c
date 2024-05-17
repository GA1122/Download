FileWriterSync* DOMFileSystemSync::createWriter(const FileEntrySync* fileEntry, ExceptionState& exceptionState)
{
    ASSERT(fileEntry);

    FileWriterSync* fileWriter = FileWriterSync::create();
    OwnPtr<ReceiveFileWriterCallback> successCallback = ReceiveFileWriterCallback::create();
    FileError::ErrorCode errorCode = FileError::OK;
    OwnPtr<LocalErrorCallback> errorCallback = LocalErrorCallback::create(errorCode);

    OwnPtr<AsyncFileSystemCallbacks> callbacks = FileWriterBaseCallbacks::create(fileWriter, successCallback.release(), errorCallback.release(), m_context);
    callbacks->setShouldBlockUntilCompletion(true);

    fileSystem()->createFileWriter(createFileSystemURL(fileEntry), fileWriter, callbacks.release());
    if (errorCode != FileError::OK) {
        FileError::throwDOMException(exceptionState, errorCode);
        return 0;
    }
    return fileWriter;
}
