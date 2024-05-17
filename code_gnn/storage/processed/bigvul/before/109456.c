void BlobData::appendURL(const KURL& url, long long offset, long long length, double expectedModificationTime)
{
    m_items.append(BlobDataItem(url, offset, length, expectedModificationTime));
}
