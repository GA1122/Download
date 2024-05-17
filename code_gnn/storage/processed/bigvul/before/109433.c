FileReaderLoader::FileReaderLoader(ReadType readType, FileReaderLoaderClient* client)
    : m_readType(readType)
    , m_client(client)
    , m_isRawDataConverted(false)
    , m_stringResult("")
    , m_variableLength(false)
    , m_bytesLoaded(0)
    , m_totalBytes(0)
    , m_hasRange(false)
    , m_rangeStart(0)
    , m_rangeEnd(0)
    , m_errorCode(0)
{
}
