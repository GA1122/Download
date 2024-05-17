void BlobData::detachFromCurrentThread()
{
    m_contentType = m_contentType.isolatedCopy();
    m_contentDisposition = m_contentDisposition.isolatedCopy();
    for (size_t i = 0; i < m_items.size(); ++i)
        m_items.at(i).detachFromCurrentThread();
}
