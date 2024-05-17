    static PassOwnPtr<LocalErrorCallback> create(FileError::ErrorCode& errorCode)
    {
        return adoptPtr(new LocalErrorCallback(errorCode));
    }
