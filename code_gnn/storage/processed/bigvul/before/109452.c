void BlobData::appendBlob(const KURL& url, long long offset, long long length)
{
    m_items.append(BlobDataItem(url, offset, length));
}
