FileError::ErrorCode FileReaderLoader::httpStatusCodeToErrorCode(int httpStatusCode)
{
    switch (httpStatusCode) {
    case 403:
        return FileError::SECURITY_ERR;
    case 404:
        return FileError::NOT_FOUND_ERR;
    default:
        return FileError::NOT_READABLE_ERR;
    }
}
