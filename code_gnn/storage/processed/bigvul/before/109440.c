void FileReaderLoader::didFail(const ResourceError&)
{
    if (m_errorCode == FileError::ABORT_ERR)
        return;

    failed(FileError::NOT_READABLE_ERR);
}
