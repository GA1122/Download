void GIFImageReader::setRemainingBytes(size_t remainingBytes)
{
    ASSERT(remainingBytes <= m_data->size());
    m_bytesRead = m_data->size() - remainingBytes;
}
