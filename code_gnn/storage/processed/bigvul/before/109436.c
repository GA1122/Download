void FileReaderLoader::cancel()
{
    m_errorCode = FileError::ABORT_ERR;
    terminate();
}
